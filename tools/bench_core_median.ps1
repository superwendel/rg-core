param(
	[ValidateRange(1, 99)]
	[int]$Runs = 3,

	[switch]$SkipBuild,

	[string]$OutputPath
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$invariantCulture = [System.Globalization.CultureInfo]::InvariantCulture
$repoRoot = Split-Path -Parent $PSScriptRoot

function Convert-ToNumber([string]$Text)
{
	return [double]::Parse($Text, $invariantCulture)
}

function Add-Columns(
	[hashtable]$Results,
	[string]$Prefix,
	[string[]]$Headers,
	[string[]]$Tokens,
	[int]$ValueOffset)
{
	if ($Tokens.Count -ne $Headers.Count + $ValueOffset)
	{
		throw "Unexpected benchmark row: $($Tokens -join ' ')"
	}

	for ($i = 0; $i -lt $Headers.Count; $i++)
	{
		$Results["$Prefix.$($Headers[$i])"] = Convert-ToNumber $Tokens[$i + $ValueOffset]
	}
}

function Convert-AlgoOutput([string]$Text)
{
	$results = @{}
	$section = ""
	[string[]]$headers = @()

	foreach ($line in ($Text -split "`r?`n"))
	{
		if ($line -like "int32 sort median*")
		{
			$section = "sort"
			continue
		}
		if ($line -like "int32 nth_element median*")
		{
			$section = "nth"
			continue
		}
		if ($line -like "32-byte record sort, 16-bit*")
		{
			$section = "record16"
			continue
		}
		if ($line -like "32-byte record sort, full-range*")
		{
			$section = "record64"
			continue
		}

		$tokens = @($line.Trim() -split "\s+")
		if ($tokens.Count -eq 0 -or $tokens[0] -eq "")
		{
			continue
		}
		if ($tokens[0] -eq "N")
		{
			$headerOffset = if ($section -eq "sort" -or $section -eq "nth") { 2 } else { 1 }
			$headers = @($tokens[$headerOffset..($tokens.Count - 1)])
			continue
		}
		if ($tokens[0] -ne "1048576")
		{
			continue
		}

		if ($section -eq "sort" -or $section -eq "nth")
		{
			Add-Columns $results "algo.$section.$($tokens[1])" $headers $tokens 2
		}
		elseif ($section -eq "record16" -or $section -eq "record64")
		{
			Add-Columns $results "algo.$section" $headers $tokens 1
		}
	}

	if (-not $results.ContainsKey("algo.sort.random.rg_sort"))
	{
		throw "Could not parse rg_algo benchmark output."
	}
	return $results
}

function Convert-HashOutput([string]$Text)
{
	$results = @{}
	foreach ($operation in @("insert", "find", "remove"))
	{
		$pattern = "(?m)^\s*${operation}:\s+rg ([0-9.]+) ms \| std ([0-9.]+) ms(?: \| stb_ds ([0-9.]+) ms)?"
		$match = [regex]::Match($Text, $pattern)
		if (-not $match.Success)
		{
			throw "Could not parse rg_hash $operation result."
		}
		$results["hash.$operation.rg"] = Convert-ToNumber $match.Groups[1].Value
		$results["hash.$operation.std"] = Convert-ToNumber $match.Groups[2].Value
		if ($match.Groups[3].Success)
		{
			$results["hash.$operation.stb_ds"] = Convert-ToNumber $match.Groups[3].Value
		}
	}
	return $results
}

function Convert-ContainerOutput([string]$Text)
{
	$results = @{}
	$patterns = [ordered]@{
		"containers.array_reserve" = "(?m)^\s*reserve:\s+rg ([0-9.]+) ms \| std ([0-9.]+) ms(?: \| stb_ds ([0-9.]+) ms)?"
		"containers.array_grow" = "(?m)^\s*grow:\s+rg ([0-9.]+) ms \| std ([0-9.]+) ms(?: \| stb_ds ([0-9.]+) ms)?"
		"containers.smallvec" = "(?m)^\s*rg_smallvec ([0-9.]+) ms \| std::vector ([0-9.]+) ms"
		"containers.ring" = "(?m)^\s*rg_ring ([0-9.]+) ms \| std::deque ([0-9.]+) ms"
		"containers.sparse_insert" = "(?m)^\s*insert:\s+rg ([0-9.]+) ms \| entt ([0-9.]+) ms"
		"containers.sparse_remove" = "(?m)^\s*remove:\s+rg ([0-9.]+) ms \| entt ([0-9.]+) ms"
	}

	foreach ($entry in $patterns.GetEnumerator())
	{
		$match = [regex]::Match($Text, $entry.Value)
		if (-not $match.Success)
		{
			if ($entry.Key -like "*.sparse_*")
			{
				continue
			}
			throw "Could not parse $($entry.Key) result."
		}

		$results["$($entry.Key).rg"] = Convert-ToNumber $match.Groups[1].Value
		$comparisonName = if ($entry.Key -eq "containers.ring") { "std_deque" } `
			elseif ($entry.Key -like "*.sparse_*") { "entt" } else { "std_vector" }
		$results["$($entry.Key).$comparisonName"] = Convert-ToNumber $match.Groups[2].Value
		if ($match.Groups.Count -gt 3 -and $match.Groups[3].Success)
		{
			$results["$($entry.Key).stb_ds"] = Convert-ToNumber $match.Groups[3].Value
		}
	}
	return $results
}

function Invoke-Benchmark([string]$Executable)
{
	$startInfo = [System.Diagnostics.ProcessStartInfo]::new()
	$startInfo.FileName = $Executable
	$startInfo.WorkingDirectory = $repoRoot
	$startInfo.UseShellExecute = $false
	$startInfo.CreateNoWindow = $true
	$startInfo.RedirectStandardOutput = $true
	$startInfo.RedirectStandardError = $true

	$process = [System.Diagnostics.Process]::new()
	$process.StartInfo = $startInfo
	[void]$process.Start()
	$standardOutput = $process.StandardOutput.ReadToEnd()
	$standardError = $process.StandardError.ReadToEnd()
	$process.WaitForExit()
	if ($process.ExitCode -ne 0)
	{
		throw "$Executable failed with exit code $($process.ExitCode): $standardError"
	}
	return $standardOutput
}

function Get-MedianValue([object[]]$ProcessResults, [string]$Key)
{
	[double[]]$values = @()
	foreach ($result in $ProcessResults)
	{
		if (-not $result.ContainsKey($Key))
		{
			throw "Benchmark result is missing $Key."
		}
		$values += [double]$result[$Key]
	}
	[Array]::Sort($values)
	$middle = [int][Math]::Floor($values.Count / 2)
	if (($values.Count % 2) -eq 1)
	{
		return $values[$middle]
	}
	return ($values[$middle - 1] + $values[$middle]) / 2.0
}

function Format-Number([double]$Value, [int]$Digits)
{
	return $Value.ToString("F$Digits", $invariantCulture)
}

function Add-Table(
	[System.Collections.Generic.List[string]]$Report,
	[string[]]$Headers,
	[object[]]$Rows)
{
	[void]$Report.Add("| " + ($Headers -join " | ") + " |")
	[void]$Report.Add("| " + (($Headers | ForEach-Object { "---" }) -join " | ") + " |")
	foreach ($row in $Rows)
	{
		[void]$Report.Add("| " + ($row -join " | ") + " |")
	}
	[void]$Report.Add("")
}

Push-Location $repoRoot
try
{
	if (-not $SkipBuild)
	{
		$previousBuildOnly = [Environment]::GetEnvironmentVariable("RG_BENCH_BUILD_ONLY", "Process")
		try
		{
			[Environment]::SetEnvironmentVariable("RG_BENCH_BUILD_ONLY", "1", "Process")
			& "$repoRoot\build.bat" bench
			if ($LASTEXITCODE -ne 0)
			{
				throw "Benchmark build failed with exit code $LASTEXITCODE."
			}
		}
		finally
		{
			[Environment]::SetEnvironmentVariable("RG_BENCH_BUILD_ONLY", $previousBuildOnly, "Process")
		}
	}

	$algoRuns = @()
	$hashRuns = @()
	$containerRuns = @()
	for ($run = 1; $run -le $Runs; $run++)
	{
		Write-Host "rg_algo process $run/$Runs"
		$algoRuns += ,(Convert-AlgoOutput (Invoke-Benchmark "$repoRoot\bench_algo.exe"))
		Write-Host "rg_hash process $run/$Runs"
		$hashRuns += ,(Convert-HashOutput (Invoke-Benchmark "$repoRoot\bench_hash.exe"))
		Write-Host "rg_containers process $run/$Runs"
		$containerRuns += ,(Convert-ContainerOutput (Invoke-Benchmark "$repoRoot\bench_containers.exe"))
	}

	$report = [System.Collections.Generic.List[string]]::new()
	[void]$report.Add("# rg_core benchmark outer medians")
	[void]$report.Add("")
	[void]$report.Add("Each cell is the median of $Runs fresh process runs. Each process performs one warmup and reports the median of seven samples.")
	[void]$report.Add("")
	[void]$report.Add("## rg_algo int32 sort, 1,048,576 values (ms)")
	[void]$report.Add("")
	$sortColumns = @("rg_sort", "rg_radix", "std_sort", "qsort", "quadsort", "crumsort", "rg_stable", "std_stable") | Where-Object { $algoRuns[0].ContainsKey("algo.sort.random.$_") }
	$sortHeaders = @("Pattern") + @($sortColumns)
	$sortRows = @()
	foreach ($pattern in @("random", "sorted", "reverse", "nearly", "dupes256", "all_equal"))
	{
		$row = @($pattern)
		foreach ($column in $sortColumns)
		{
			$row += Format-Number ((Get-MedianValue $algoRuns "algo.sort.$pattern.$column") / 1000.0) 3
		}
		$sortRows += ,$row
	}
	Add-Table $report $sortHeaders $sortRows

	[void]$report.Add("## rg_algo nth_element, 1,048,576 values (ms)")
	[void]$report.Add("")
	$nthRows = @(
		@("random", (Format-Number ((Get-MedianValue $algoRuns "algo.nth.random.rg_nth") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.nth.random.std_nth") / 1000.0) 3)),
		@("dupes256", (Format-Number ((Get-MedianValue $algoRuns "algo.nth.dupes256.rg_nth") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.nth.dupes256.std_nth") / 1000.0) 3))
	)
	Add-Table $report @("Pattern", "rg_nth", "std_nth") $nthRows

	[void]$report.Add("## rg_algo 32-byte records, 1,048,576 values (ms)")
	[void]$report.Add("")
	$recordRows = @(
		@("32-byte record, 16-bit key", (Format-Number ((Get-MedianValue $algoRuns "algo.record16.rg_sort") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record16.rg_radix") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record16.std_sort") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record16.qsort") / 1000.0) 3)),
		@("32-byte record, 64-bit key", (Format-Number ((Get-MedianValue $algoRuns "algo.record64.rg_sort") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record64.rg_radix") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record64.std_sort") / 1000.0) 3), (Format-Number ((Get-MedianValue $algoRuns "algo.record64.qsort") / 1000.0) 3))
	)
	Add-Table $report @("Workload", "rg_sort", "rg_radix", "std_sort", "qsort") $recordRows

	[void]$report.Add("## rg_hash (ms)")
	[void]$report.Add("")
	$hashColumns = @("rg", "std", "stb_ds") | Where-Object { $hashRuns[0].ContainsKey("hash.insert.$_") }
	$hashRows = @()
	foreach ($operation in @("insert", "find", "remove"))
	{
		$row = @($operation)
		foreach ($column in $hashColumns)
		{
			$row += Format-Number (Get-MedianValue $hashRuns "hash.$operation.$column") 2
		}
		$hashRows += ,$row
	}
	Add-Table $report (@("Operation") + $hashColumns) $hashRows

	[void]$report.Add("## rg_containers (ms)")
	[void]$report.Add("")
	$containerRows = @()
	foreach ($spec in @(
		@("array reserve", "containers.array_reserve", "std_vector"),
		@("array grow", "containers.array_grow", "std_vector"),
		@("smallvec", "containers.smallvec", "std_vector"),
		@("ring", "containers.ring", "std_deque"),
		@("sparse insert", "containers.sparse_insert", "entt"),
		@("sparse remove", "containers.sparse_remove", "entt")
	))
	{
		if (-not $containerRuns[0].ContainsKey("$($spec[1]).rg"))
		{
			continue
		}
		$row = @($spec[0], (Format-Number (Get-MedianValue $containerRuns "$($spec[1]).rg") 2), (Format-Number (Get-MedianValue $containerRuns "$($spec[1]).$($spec[2])") 2))
		if ($containerRuns[0].ContainsKey("$($spec[1]).stb_ds"))
		{
			$row += Format-Number (Get-MedianValue $containerRuns "$($spec[1]).stb_ds") 2
		}
		else
		{
			$row += "n/a"
		}
		$containerRows += ,$row
	}
	Add-Table $report @("Workload", "rg", "standard/EnTT", "stb_ds") $containerRows

	$reportText = $report -join [Environment]::NewLine
	Write-Output $reportText
	if ($OutputPath)
	{
		Set-Content -LiteralPath $OutputPath -Value $reportText -Encoding utf8
	}
}
finally
{
	Pop-Location
}
