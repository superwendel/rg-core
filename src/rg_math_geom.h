// rg_math_geom - Geometry and intersection helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides geometric primitives, bounds, intersections, and distance helpers.
//
// USAGE:
//   #include "rg_math_geom.h"
//
//   rg_ray ray;
//   rg_ray_set(&ray, &origin, &dir);
//   if (rg_ray_sphere(&ray, &sphere, &t1, &t2)) { ... }
//
// Author: Steven Wendel (superwendel)
//
// NOTES:
//   - Ray directions should be normalized for correct t distances.
//   - Plane equation: dot(n, x) + d = 0.

#ifndef RG_MATH_GEOM_H
#define RG_MATH_GEOM_H

#include "rg_math_mat.h"

#include <float.h>

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// TYPE DEFINITIONS
// =============================================================================

/**
 * @brief Ray (origin + direction)
 */
typedef struct rg_ray
{
	rg_vec3 origin;
	rg_vec3 dir;
} rg_ray;

/**
 * @brief Axis-aligned bounding box
 */
typedef struct rg_aabb
{
	rg_vec3 min;
	rg_vec3 max;
} rg_aabb;

/**
 * @brief 2D axis-aligned bounding box
 */
typedef struct rg_aabb2
{
	rg_vec2 min;
	rg_vec2 max;
} rg_aabb2;

/**
 * @brief Sphere (center + radius)
 */
typedef struct rg_sphere
{
	rg_vec3 center;
	f32 radius;
} rg_sphere;

/**
 * @brief Circle (center + radius)
 */
typedef struct rg_circle
{
	rg_vec2 center;
	f32 radius;
} rg_circle;

/**
 * @brief Plane (normal + distance)
 */
typedef struct rg_plane
{
	rg_vec3 normal;
	f32 d;
} rg_plane;

#define RG_FRUSTUM_PLANE_COUNT 6
#define RG_FRUSTUM_LEFT 0
#define RG_FRUSTUM_RIGHT 1
#define RG_FRUSTUM_BOTTOM 2
#define RG_FRUSTUM_TOP 3
#define RG_FRUSTUM_NEAR 4
#define RG_FRUSTUM_FAR 5

#define RG_FRUSTUM_CORNER_COUNT 8
#define RG_FRUSTUM_CORNER_LBN 0
#define RG_FRUSTUM_CORNER_LTN 1
#define RG_FRUSTUM_CORNER_RTN 2
#define RG_FRUSTUM_CORNER_RBN 3
#define RG_FRUSTUM_CORNER_LBF 4
#define RG_FRUSTUM_CORNER_LTF 5
#define RG_FRUSTUM_CORNER_RTF 6
#define RG_FRUSTUM_CORNER_RBF 7

#define RG_PLANE_OUTSIDE 0
#define RG_PLANE_INTERSECT 1
#define RG_PLANE_INSIDE 2

/**
 * @brief View frustum (6 planes)
 */
typedef struct rg_frustum
{
	rg_plane planes[RG_FRUSTUM_PLANE_COUNT];
} rg_frustum;

// =============================================================================
// RAY HELPERS
// =============================================================================

/**
 * @brief Set ray origin/direction
 * @param ray Output ray
 * @param origin Ray origin
 * @param dir Ray direction (normalized)
 */
RGINLINE void rg_ray_set(rg_ray* ray, const rg_vec3* origin, const rg_vec3* dir);

/**
 * @brief Point along ray at t
 * @param ray Ray
 * @param t Parameter
 * @param out Output point
 */
RGINLINE void rg_ray_at(const rg_ray* ray, f32 t, rg_vec3* out);

// =============================================================================
// LINE HELPERS
// =============================================================================

/**
 * @brief Closest point on line segment AB to point p (clamped)
 * @param p Point
 * @param a Segment start
 * @param b Segment end
 * @param out Output closest point
 */
RGINLINE void rg_line_closest_point(const rg_vec3* p, const rg_vec3* a, const rg_vec3* b, rg_vec3* out);

/**
 * @brief Closest point on 2D line segment AB to point p (clamped)
 * @param p Point
 * @param a Segment start
 * @param b Segment end
 * @param out Output closest point
 */
RGINLINE void rg_line2_closest_point(const rg_vec2* p, const rg_vec2* a, const rg_vec2* b, rg_vec2* out);

// =============================================================================
// AABB HELPERS
// =============================================================================

/**
 * @brief Set AABB min/max
 * @param aabb Output AABB
 * @param min Min corner
 * @param max Max corner
 */
RGINLINE void rg_aabb_set(rg_aabb* aabb, const rg_vec3* min, const rg_vec3* max);

/**
 * @brief Build AABB from center and extents
 * @param aabb Output AABB
 * @param center Center point
 * @param extents Half-size along each axis
 */
RGINLINE void rg_aabb_from_center_extents(rg_aabb* aabb, const rg_vec3* center, const rg_vec3* extents);

/**
 * @brief Expand AABB to include a point
 * @param aabb AABB to expand
 * @param p Point to include
 */
RGINLINE void rg_aabb_expand_point(rg_aabb* aabb, const rg_vec3* p);

/**
 * @brief Union of two AABBs
 * @param a First AABB
 * @param b Second AABB
 * @param out Output union AABB
 */
RGINLINE void rg_aabb_union(const rg_aabb* a, const rg_aabb* b, rg_aabb* out);

/**
 * @brief Merge two AABBs (alias for union)
 * @param a First AABB
 * @param b Second AABB
 * @param out Output merged AABB
 */
RGINLINE void rg_aabb_merge(const rg_aabb* a, const rg_aabb* b, rg_aabb* out);

/**
 * @brief Crop AABB by another AABB (intersection)
 * @param aabb Input AABB
 * @param crop Crop AABB
 * @param out Output cropped AABB
 */
RGINLINE void rg_aabb_crop(const rg_aabb* aabb, const rg_aabb* crop, rg_aabb* out);

/**
 * @brief Crop AABB by another AABB, then clamp to a minimum AABB
 * @param aabb Input AABB
 * @param crop Crop AABB
 * @param clamp Minimum AABB
 * @param out Output cropped AABB
 */
RGINLINE void rg_aabb_crop_until(const rg_aabb* aabb, const rg_aabb* crop, const rg_aabb* clamp, rg_aabb* out);

/**
 * @brief Transform AABB by an affine matrix (no perspective)
 * @param aabb Input AABB
 * @param m Affine transform
 * @param out Output transformed AABB
 */
RGINLINE void rg_aabb_transform(const rg_aabb* aabb, const rg_mat4* m, rg_aabb* out);

/**
 * @brief Invalidate AABB (sets min to +inf, max to -inf)
 * @param aabb AABB to invalidate
 */
RGINLINE void rg_aabb_invalidate(rg_aabb* aabb);

/**
 * @brief Check if AABB is valid
 * @param aabb AABB to test
 * @return 1 if valid, 0 otherwise
 */
RGINLINE int rg_aabb_isvalid(const rg_aabb* aabb);

/**
 * @brief Diagonal length of AABB
 * @param aabb AABB
 * @return Distance between min and max
 */
RGINLINE f32 rg_aabb_diag(const rg_aabb* aabb);

/**
 * @brief Size of AABB (max - min)
 * @param aabb AABB
 * @param out Output size
 */
RGINLINE void rg_aabb_size(const rg_aabb* aabb, rg_vec3* out);

/**
 * @brief Radius of sphere that encloses AABB
 * @param aabb AABB
 * @return Radius
 */
RGINLINE f32 rg_aabb_radius(const rg_aabb* aabb);

/**
 * @brief Center point of AABB
 * @param aabb AABB
 * @param out Output center
 */
RGINLINE void rg_aabb_center(const rg_aabb* aabb, rg_vec3* out);

/**
 * @brief Check if AABB contains point
 * @param aabb AABB
 * @param p Point
 * @return 1 if inside, 0 otherwise
 */
RGINLINE int rg_aabb_contains_point(const rg_aabb* aabb, const rg_vec3* p);

/**
 * @brief Check if AABB contains another AABB
 * @param aabb AABB
 * @param other Other AABB
 * @return 1 if fully contained, 0 otherwise
 */
RGINLINE int rg_aabb_contains(const rg_aabb* aabb, const rg_aabb* other);

/**
 * @brief Check if AABBs overlap
 * @param a First AABB
 * @param b Second AABB
 * @return 1 if overlapping, 0 otherwise
 */
RGINLINE int rg_aabb_overlaps(const rg_aabb* a, const rg_aabb* b);

/**
 * @brief Closest point on AABB to point
 * @param aabb AABB
 * @param p Point
 * @param out Output closest point
 */
RGINLINE void rg_aabb_closest_point(const rg_aabb* aabb, const rg_vec3* p, rg_vec3* out);

// =============================================================================
// AABB2D HELPERS
// =============================================================================

/**
 * @brief Set AABB2D min/max
 * @param aabb Output AABB2D
 * @param min Min corner
 * @param max Max corner
 */
RGINLINE void rg_aabb2_set(rg_aabb2* aabb, const rg_vec2* min, const rg_vec2* max);

/**
 * @brief Zero AABB2D
 * @param aabb AABB2D to zero
 */
RGINLINE void rg_aabb2_zero(rg_aabb2* aabb);

/**
 * @brief Copy AABB2D
 * @param aabb Source AABB2D
 * @param out Output AABB2D
 */
RGINLINE void rg_aabb2_copy(const rg_aabb2* aabb, rg_aabb2* out);

/**
 * @brief Transform AABB2D by an affine matrix (no perspective)
 * @param aabb Input AABB2D
 * @param m Affine transform (mat3)
 * @param out Output transformed AABB2D
 */
RGINLINE void rg_aabb2_transform(const rg_aabb2* aabb, const rg_mat3* m, rg_aabb2* out);

/**
 * @brief Merge two AABB2D boxes
 * @param a First AABB2D
 * @param b Second AABB2D
 * @param out Output merged AABB2D
 */
RGINLINE void rg_aabb2_merge(const rg_aabb2* a, const rg_aabb2* b, rg_aabb2* out);

/**
 * @brief Crop AABB2D by another AABB2D
 * @param aabb Input AABB2D
 * @param crop Crop AABB2D
 * @param out Output cropped AABB2D
 */
RGINLINE void rg_aabb2_crop(const rg_aabb2* aabb, const rg_aabb2* crop, rg_aabb2* out);

/**
 * @brief Crop AABB2D then clamp to a minimum AABB2D
 * @param aabb Input AABB2D
 * @param crop Crop AABB2D
 * @param clamp Minimum AABB2D
 * @param out Output cropped AABB2D
 */
RGINLINE void rg_aabb2_crop_until(const rg_aabb2* aabb, const rg_aabb2* crop, const rg_aabb2* clamp, rg_aabb2* out);

/**
 * @brief Invalidate AABB2D (sets min to +inf, max to -inf)
 * @param aabb AABB2D to invalidate
 */
RGINLINE void rg_aabb2_invalidate(rg_aabb2* aabb);

/**
 * @brief Check if AABB2D is valid
 * @param aabb AABB2D
 * @return 1 if valid, 0 otherwise
 */
RGINLINE int rg_aabb2_isvalid(const rg_aabb2* aabb);

/**
 * @brief Diagonal length of AABB2D
 * @param aabb AABB2D
 * @return Distance between min and max
 */
RGINLINE f32 rg_aabb2_diag(const rg_aabb2* aabb);

/**
 * @brief Size of AABB2D (max - min)
 * @param aabb AABB2D
 * @param out Output size
 */
RGINLINE void rg_aabb2_size(const rg_aabb2* aabb, rg_vec2* out);

/**
 * @brief Radius of circle that encloses AABB2D
 * @param aabb AABB2D
 * @return Radius
 */
RGINLINE f32 rg_aabb2_radius(const rg_aabb2* aabb);

/**
 * @brief Center point of AABB2D
 * @param aabb AABB2D
 * @param out Output center
 */
RGINLINE void rg_aabb2_center(const rg_aabb2* aabb, rg_vec2* out);

/**
 * @brief Check if AABB2D overlaps another AABB2D
 * @param aabb AABB2D
 * @param other Other AABB2D
 * @return 1 if overlapping, 0 otherwise
 */
RGINLINE int rg_aabb2_overlaps(const rg_aabb2* aabb, const rg_aabb2* other);

/**
 * @brief Check if AABB2D overlaps a circle
 * @param aabb AABB2D
 * @param circle Circle (center + radius)
 * @return 1 if overlapping, 0 otherwise
 */
RGINLINE int rg_aabb2_circle(const rg_aabb2* aabb, const rg_circle* circle);

/**
 * @brief Check if AABB2D contains a point
 * @param aabb AABB2D
 * @param p Point
 * @return 1 if inside, 0 otherwise
 */
RGINLINE int rg_aabb2_contains_point(const rg_aabb2* aabb, const rg_vec2* p);

/**
 * @brief Check if AABB2D contains another AABB2D
 * @param aabb AABB2D
 * @param other Other AABB2D
 * @return 1 if fully contained, 0 otherwise
 */
RGINLINE int rg_aabb2_contains(const rg_aabb2* aabb, const rg_aabb2* other);

// =============================================================================
// SPHERE HELPERS
// =============================================================================

/**
 * @brief Set sphere center/radius
 * @param sphere Output sphere
 * @param center Sphere center
 * @param radius Sphere radius
 */
RGINLINE void rg_sphere_set(rg_sphere* sphere, const rg_vec3* center, f32 radius);

/**
 * @brief Transform sphere center by an affine matrix (radius unchanged)
 * @param sphere Input sphere
 * @param m Affine transform
 * @param out Output transformed sphere
 */
RGINLINE void rg_sphere_transform(const rg_sphere* sphere, const rg_mat4* m, rg_sphere* out);

/**
 * @brief Merge two spheres into a bounding union
 * @param a First sphere
 * @param b Second sphere
 * @param out Output merged sphere
 */
RGINLINE void rg_sphere_merge(const rg_sphere* a, const rg_sphere* b, rg_sphere* out);

/**
 * @brief Check if sphere contains point
 * @param sphere Sphere
 * @param p Point
 * @return 1 if inside, 0 otherwise
 */
RGINLINE int rg_sphere_contains_point(const rg_sphere* sphere, const rg_vec3* p);

/**
 * @brief Check if spheres overlap
 * @param a First sphere
 * @param b Second sphere
 * @return 1 if overlapping, 0 otherwise
 */
RGINLINE int rg_sphere_overlaps(const rg_sphere* a, const rg_sphere* b);

// =============================================================================
// PLANE HELPERS
// =============================================================================

/**
 * @brief Build plane from point and normal
 * @param plane Output plane
 * @param point Point on plane
 * @param normal Plane normal (normalized)
 */
RGINLINE void rg_plane_from_point_normal(rg_plane* plane, const rg_vec3* point, const rg_vec3* normal);

/**
 * @brief Build plane from triangle vertices
 * @param plane Output plane
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 */
RGINLINE void rg_plane_from_triangle(rg_plane* plane, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c);

/**
 * @brief Normalize plane (normal length to 1)
 * @param plane Plane to normalize
 */
RGINLINE void rg_plane_normalize(rg_plane* plane);

/**
 * @brief Signed distance from plane to point
 * @param plane Plane
 * @param p Point
 * @return Signed distance (positive in front of plane)
 */
RGINLINE f32 rg_plane_distance(const rg_plane* plane, const rg_vec3* p);

/**
 * @brief Project point onto plane
 * @param plane Plane
 * @param p Point
 * @param out Output projected point
 */
RGINLINE void rg_plane_project_point(const rg_plane* plane, const rg_vec3* p, rg_vec3* out);

// =============================================================================
// FRUSTUM HELPERS
// =============================================================================

/**
 * @brief Extract frustum planes from a matrix (left, right, bottom, top, near, far)
 * @param m Matrix (view-projection or similar)
 * @param out Output frustum
 */
RGINLINE void rg_frustum_from_mat4(const rg_mat4* m, rg_frustum* out);

/**
 * @brief AABB frustum culling test
 * @param aabb AABB
 * @param frustum Frustum planes
 * @return 1 if intersecting/inside, 0 if outside
 */
RGINLINE int rg_aabb_in_frustum(const rg_aabb* aabb, const rg_frustum* frustum);

/**
 * @brief Extract frustum corners from inverse view-projection (clipspace control)
 * @param inv_mat Inverse view-projection (or inverse projection)
 * @param out Output corners (RG_FRUSTUM_CORNER_*)
 */
RGINLINE void rg_frustum_corners(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT]);

/**
 * @brief Extract frustum corners for -1..1 depth clip space
 * @param inv_mat Inverse view-projection (or inverse projection)
 * @param out Output corners (RG_FRUSTUM_CORNER_*)
 */
RGINLINE void rg_frustum_corners_no(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT]);

/**
 * @brief Extract frustum corners for 0..1 depth clip space
 * @param inv_mat Inverse view-projection (or inverse projection)
 * @param out Output corners (RG_FRUSTUM_CORNER_*)
 */
RGINLINE void rg_frustum_corners_zo(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT]);

/**
 * @brief Compute frustum center from its corners
 * @param corners Frustum corners
 * @param out Output center
 */
RGINLINE void rg_frustum_center(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], rg_vec4* out);

/**
 * @brief Compute AABB for frustum corners in a given space
 * @param corners Frustum corners
 * @param m Matrix to transform corners (e.g., view)
 * @param out Output AABB
 */
RGINLINE void rg_frustum_box(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], const rg_mat4* m, rg_aabb* out);

/**
 * @brief Compute frustum split plane corners between near/far planes
 * @param corners Frustum corners (RG_FRUSTUM_CORNER_*)
 * @param split_dist Split distance along the view direction
 * @param far_dist Far distance of the frustum
 * @param out Output plane corners (LB, LT, RT, RB)
 */
RGINLINE void rg_frustum_corners_at(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], f32 split_dist, f32 far_dist, rg_vec4 out[4]);

// =============================================================================
// INTERSECTION TESTS
// =============================================================================

/**
 * @brief Ray-plane intersection
 * @param ray Ray (dir normalized)
 * @param plane Plane (normal normalized)
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_plane(const rg_ray* ray, const rg_plane* plane, f32* t);

/**
 * @brief Fast ray-plane intersection (no epsilon check)
 * @param ray Ray (dir normalized, non-parallel)
 * @param plane Plane (normal normalized)
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 * @note Assumes vec3 padding is zero and denom != 0.0f.
 */
RGINLINE int rg_ray_plane_fast(const rg_ray* ray, const rg_plane* plane, f32* t);

/**
 * @brief Ray-plane intersection (point + normal)
 * @param ray Ray (dir normalized)
 * @param point Point on plane
 * @param normal Plane normal (normalized)
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_plane_point_normal(const rg_ray* ray, const rg_vec3* point, const rg_vec3* normal, f32* t);

/**
 * @brief Fast ray-plane intersection (point + normal, no epsilon check)
 * @param ray Ray (dir normalized, non-parallel)
 * @param point Point on plane
 * @param normal Plane normal (normalized)
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 * @note Assumes vec3 padding is zero and denom != 0.0f.
 */
RGINLINE int rg_ray_plane_point_normal_fast(const rg_ray* ray, const rg_vec3* point, const rg_vec3* normal, f32* t);

/**
 * @brief Ray-sphere intersection (Press/Teukolsky stable form)
 * @param ray Ray (dir normalized)
 * @param sphere Sphere
 * @param t1 Near hit distance (optional)
 * @param t2 Far hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_sphere(const rg_ray* ray, const rg_sphere* sphere, f32* t1, f32* t2);

/**
 * @brief Ray-sphere intersection (closest hit distance)
 * @param ray Ray (dir normalized)
 * @param sphere Sphere
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_sphere_distance(const rg_ray* ray, const rg_sphere* sphere, f32* t);

/**
 * @brief Ray-triangle intersection (Moller-Trumbore)
 * @param ray Ray (dir normalized)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_triangle(const rg_ray* ray, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, f32* t);

/**
 * @brief Ray-triangle intersection (Moller-Trumbore) with barycentric coords
 * @param ray Ray (dir normalized)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @param bary Output barycentric coords (u, v) for vertices B/C (optional)
 * @param t Hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_triangle_bary(const rg_ray* ray, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, rg_vec2* bary, f32* t);

/**
 * @brief Line-triangle intersection (Moller-Trumbore)
 * @param origin Line origin
 * @param dir Line direction (normalized for distance)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @param bary Output barycentric coords (u, v) for vertices B/C (optional)
 * @param t Hit distance along line (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_line_triangle(const rg_vec3* origin, const rg_vec3* dir, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, rg_vec2* bary, f32* t);

/**
 * @brief Line-sphere intersection (returns two points and normals)
 * @param p0 First point on line
 * @param p1 Second point on line
 * @param sphere Sphere
 * @param out_p1 Output intersection point 1 (optional)
 * @param out_n1 Output intersection normal 1 (optional)
 * @param out_p2 Output intersection point 2 (optional)
 * @param out_n2 Output intersection normal 2 (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_line_sphere(const rg_vec3* p0, const rg_vec3* p1, const rg_sphere* sphere, rg_vec3* out_p1, rg_vec3* out_n1, rg_vec3* out_p2, rg_vec3* out_n2);

/**
 * @brief Ray-AABB intersection (slab method)
 * @param ray Ray
 * @param aabb AABB
 * @param tmin Near hit distance (optional)
 * @param tmax Far hit distance (optional)
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_ray_aabb(const rg_ray* ray, const rg_aabb* aabb, f32* tmin, f32* tmax);

/**
 * @brief AABB-plane classification
 * @param aabb AABB
 * @param plane Plane
 * @return RG_PLANE_OUTSIDE, RG_PLANE_INTERSECT, or RG_PLANE_INSIDE
 */
RGINLINE int rg_aabb_plane(const rg_aabb* aabb, const rg_plane* plane);

/**
 * @brief AABB-sphere intersection
 * @param aabb AABB
 * @param sphere Sphere
 * @return 1 if intersects, 0 otherwise
 */
RGINLINE int rg_aabb_sphere(const rg_aabb* aabb, const rg_sphere* sphere);

/**
 * @brief Barycentric coordinates for point in triangle
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @param p Point
 * @param out Output barycentric coordinates (u, v, w)
 */
RGINLINE void rg_barycentric(const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, const rg_vec3* p, rg_vec3* out);

/**
 * @brief Check if point lies in triangle (barycentric test)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @param p Point
 * @return 1 if inside, 0 otherwise
 */
RGINLINE int rg_point_in_triangle(const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, const rg_vec3* p);

RG_MATH_EXTERN_C_END

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE void rg_ray_set(rg_ray* ray, const rg_vec3* origin, const rg_vec3* dir)
{
	ray->origin = *origin;
	ray->dir = *dir;
}

RGINLINE void rg_ray_at(const rg_ray* ray, f32 t, rg_vec3* out)
{
	out->x = ray->origin.x + ray->dir.x * t;
	out->y = ray->origin.y + ray->dir.y * t;
	out->z = ray->origin.z + ray->dir.z * t;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_line_closest_point(const rg_vec3* p, const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	f32 abx = b->x - a->x;
	f32 aby = b->y - a->y;
	f32 abz = b->z - a->z;
	f32 len_sq = abx * abx + aby * aby + abz * abz;

	if (len_sq <= RG_EPSILON * RG_EPSILON)
	{
		out->x = a->x;
		out->y = a->y;
		out->z = a->z;
#if !RG_MATH_MAX_PERF
		out->_pad = 0.0f;
#endif
		return;
	}

	f32 apx = p->x - a->x;
	f32 apy = p->y - a->y;
	f32 apz = p->z - a->z;
	f32 t = (apx * abx + apy * aby + apz * abz) / len_sq;

	if (t < 0.0f)
	{
		t = 0.0f;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
	}

	out->x = a->x + abx * t;
	out->y = a->y + aby * t;
	out->z = a->z + abz * t;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_line2_closest_point(const rg_vec2* p, const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	f32 abx = b->x - a->x;
	f32 aby = b->y - a->y;
	f32 len_sq = abx * abx + aby * aby;

	if (len_sq <= RG_EPSILON * RG_EPSILON)
	{
		out->x = a->x;
		out->y = a->y;
		return;
	}

	f32 apx = p->x - a->x;
	f32 apy = p->y - a->y;
	f32 t = (apx * abx + apy * aby) / len_sq;

	if (t < 0.0f)
	{
		t = 0.0f;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
	}

	out->x = a->x + abx * t;
	out->y = a->y + aby * t;
}

RGINLINE void rg_aabb_set(rg_aabb* aabb, const rg_vec3* min, const rg_vec3* max)
{
	aabb->min = *min;
	aabb->max = *max;
}

RGINLINE void rg_aabb_from_center_extents(rg_aabb* aabb, const rg_vec3* center, const rg_vec3* extents)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c = RG_VEC3_LOAD(center);
	__m128 e = RG_VEC3_LOAD(extents);
	RG_VEC3_STORE(&aabb->min, _mm_sub_ps(c, e));
	RG_VEC3_STORE(&aabb->max, _mm_add_ps(c, e));
#else
	aabb->min.x = center->x - extents->x;
	aabb->min.y = center->y - extents->y;
	aabb->min.z = center->z - extents->z;
	aabb->max.x = center->x + extents->x;
	aabb->max.y = center->y + extents->y;
	aabb->max.z = center->z + extents->z;
#endif
}

RGINLINE void rg_aabb_expand_point(rg_aabb* aabb, const rg_vec3* p)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 pv = RG_VEC3_LOAD(p);
	RG_VEC3_STORE(&aabb->min, _mm_min_ps(RG_VEC3_LOAD(&aabb->min), pv));
	RG_VEC3_STORE(&aabb->max, _mm_max_ps(RG_VEC3_LOAD(&aabb->max), pv));
#else
	if (p->x < aabb->min.x) aabb->min.x = p->x;
	if (p->y < aabb->min.y) aabb->min.y = p->y;
	if (p->z < aabb->min.z) aabb->min.z = p->z;
	if (p->x > aabb->max.x) aabb->max.x = p->x;
	if (p->y > aabb->max.y) aabb->max.y = p->y;
	if (p->z > aabb->max.z) aabb->max.z = p->z;
#endif
}

RGINLINE void rg_aabb_union(const rg_aabb* a, const rg_aabb* b, rg_aabb* out)
{
#ifdef RG_MATH_SSE
	__m128 amin = RG_VEC3_LOAD(&a->min);
	__m128 amax = RG_VEC3_LOAD(&a->max);
	__m128 bmin = RG_VEC3_LOAD(&b->min);
	__m128 bmax = RG_VEC3_LOAD(&b->max);
	RG_VEC3_STORE(&out->min, _mm_min_ps(amin, bmin));
	RG_VEC3_STORE(&out->max, _mm_max_ps(amax, bmax));
#else
	out->min.x = rg_minf(a->min.x, b->min.x);
	out->min.y = rg_minf(a->min.y, b->min.y);
	out->min.z = rg_minf(a->min.z, b->min.z);
	out->max.x = rg_maxf(a->max.x, b->max.x);
	out->max.y = rg_maxf(a->max.y, b->max.y);
	out->max.z = rg_maxf(a->max.z, b->max.z);
#endif
}

RGINLINE void rg_aabb_merge(const rg_aabb* a, const rg_aabb* b, rg_aabb* out)
{
	rg_aabb_union(a, b, out);
}

RGINLINE void rg_aabb_crop(const rg_aabb* aabb, const rg_aabb* crop, rg_aabb* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 minv = _mm_max_ps(RG_VEC3_LOAD(&aabb->min), RG_VEC3_LOAD(&crop->min));
	__m128 maxv = _mm_min_ps(RG_VEC3_LOAD(&aabb->max), RG_VEC3_LOAD(&crop->max));
	RG_VEC3_STORE(&out->min, minv);
	RG_VEC3_STORE(&out->max, maxv);
#else
	out->min.x = rg_maxf(aabb->min.x, crop->min.x);
	out->min.y = rg_maxf(aabb->min.y, crop->min.y);
	out->min.z = rg_maxf(aabb->min.z, crop->min.z);
	out->max.x = rg_minf(aabb->max.x, crop->max.x);
	out->max.y = rg_minf(aabb->max.y, crop->max.y);
	out->max.z = rg_minf(aabb->max.z, crop->max.z);
#if !RG_MATH_MAX_PERF
	out->min._pad = 0.0f;
	out->max._pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_aabb_crop_until(const rg_aabb* aabb, const rg_aabb* crop, const rg_aabb* clamp, rg_aabb* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 minv = _mm_min_ps(RG_VEC3_LOAD(&clamp->min),
	                         _mm_max_ps(RG_VEC3_LOAD(&aabb->min), RG_VEC3_LOAD(&crop->min)));
	__m128 maxv = _mm_max_ps(RG_VEC3_LOAD(&clamp->max),
	                         _mm_min_ps(RG_VEC3_LOAD(&aabb->max), RG_VEC3_LOAD(&crop->max)));
	RG_VEC3_STORE(&out->min, minv);
	RG_VEC3_STORE(&out->max, maxv);
#else
	out->min.x = rg_minf(clamp->min.x, rg_maxf(aabb->min.x, crop->min.x));
	out->min.y = rg_minf(clamp->min.y, rg_maxf(aabb->min.y, crop->min.y));
	out->min.z = rg_minf(clamp->min.z, rg_maxf(aabb->min.z, crop->min.z));
	out->max.x = rg_maxf(clamp->max.x, rg_minf(aabb->max.x, crop->max.x));
	out->max.y = rg_maxf(clamp->max.y, rg_minf(aabb->max.y, crop->max.y));
	out->max.z = rg_maxf(clamp->max.z, rg_minf(aabb->max.z, crop->max.z));
#if !RG_MATH_MAX_PERF
	out->min._pad = 0.0f;
	out->max._pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_aabb_transform(const rg_aabb* aabb, const rg_mat4* m, rg_aabb* out)
{
	f32 cx = (aabb->min.x + aabb->max.x) * 0.5f;
	f32 cy = (aabb->min.y + aabb->max.y) * 0.5f;
	f32 cz = (aabb->min.z + aabb->max.z) * 0.5f;

	f32 ex = (aabb->max.x - aabb->min.x) * 0.5f;
	f32 ey = (aabb->max.y - aabb->min.y) * 0.5f;
	f32 ez = (aabb->max.z - aabb->min.z) * 0.5f;

	f32 ox = m->m[0] * cx + m->m[4] * cy + m->m[8] * cz + m->m[12];
	f32 oy = m->m[1] * cx + m->m[5] * cy + m->m[9] * cz + m->m[13];
	f32 oz = m->m[2] * cx + m->m[6] * cy + m->m[10] * cz + m->m[14];

	f32 rx = rg_absf(m->m[0]) * ex + rg_absf(m->m[4]) * ey + rg_absf(m->m[8]) * ez;
	f32 ry = rg_absf(m->m[1]) * ex + rg_absf(m->m[5]) * ey + rg_absf(m->m[9]) * ez;
	f32 rz = rg_absf(m->m[2]) * ex + rg_absf(m->m[6]) * ey + rg_absf(m->m[10]) * ez;

	out->min.x = ox - rx;
	out->min.y = oy - ry;
	out->min.z = oz - rz;
	out->max.x = ox + rx;
	out->max.y = oy + ry;
	out->max.z = oz + rz;
}

RGINLINE void rg_aabb_invalidate(rg_aabb* aabb)
{
	aabb->min.x = FLT_MAX;
	aabb->min.y = FLT_MAX;
	aabb->min.z = FLT_MAX;
	aabb->max.x = -FLT_MAX;
	aabb->max.y = -FLT_MAX;
	aabb->max.z = -FLT_MAX;
#if !RG_MATH_MAX_PERF
	aabb->min._pad = 0.0f;
	aabb->max._pad = 0.0f;
#endif
}

RGINLINE int rg_aabb_isvalid(const rg_aabb* aabb)
{
	return rg_vec3_max_component(&aabb->min) != FLT_MAX && rg_vec3_min_component(&aabb->max) != -FLT_MAX;
}

RGINLINE f32 rg_aabb_diag(const rg_aabb* aabb)
{
	return rg_vec3_distance(&aabb->min, &aabb->max);
}

RGINLINE void rg_aabb_size(const rg_aabb* aabb, rg_vec3* out)
{
	rg_vec3_sub(&aabb->max, &aabb->min, out);
}

RGINLINE f32 rg_aabb_radius(const rg_aabb* aabb)
{
#if defined(RG_MATH_SSE41)
	__m128 d = _mm_sub_ps(RG_VEC3_LOAD(&aabb->max), RG_VEC3_LOAD(&aabb->min));
	__m128 len_sq = _mm_dp_ps(d, d, 0x71);
	return _mm_cvtss_f32(_mm_mul_ss(_mm_sqrt_ss(len_sq), _mm_set_ss(0.5f)));
#else
	f32 dx = aabb->max.x - aabb->min.x;
	f32 dy = aabb->max.y - aabb->min.y;
	f32 dz = aabb->max.z - aabb->min.z;
	return rg_sqrtf(dx * dx + dy * dy + dz * dz) * 0.5f;
#endif
}

RGINLINE void rg_aabb_center(const rg_aabb* aabb, rg_vec3* out)
{
	rg_vec3_center(&aabb->min, &aabb->max, out);
}

RGINLINE int rg_aabb_contains_point(const rg_aabb* aabb, const rg_vec3* p)
{
	return (p->x >= aabb->min.x && p->x <= aabb->max.x) && (p->y >= aabb->min.y && p->y <= aabb->max.y) && (p->z >= aabb->min.z && p->z <= aabb->max.z);
}

RGINLINE int rg_aabb_contains(const rg_aabb* aabb, const rg_aabb* other)
{
	return (aabb->min.x <= other->min.x && aabb->max.x >= other->max.x) && (aabb->min.y <= other->min.y && aabb->max.y >= other->max.y) && (aabb->min.z <= other->min.z && aabb->max.z >= other->max.z);
}

RGINLINE int rg_aabb_overlaps(const rg_aabb* a, const rg_aabb* b)
{
#ifdef RG_MATH_SSE
	__m128 amin = RG_VEC3_LOAD(&a->min);
	__m128 amax = RG_VEC3_LOAD(&a->max);
	__m128 bmin = RG_VEC3_LOAD(&b->min);
	__m128 bmax = RG_VEC3_LOAD(&b->max);
	__m128 le = _mm_cmple_ps(amin, bmax);
	__m128 ge = _mm_cmpge_ps(amax, bmin);
	return (_mm_movemask_ps(_mm_and_ps(le, ge)) & 0x7) == 0x7;
#else
	return (a->min.x <= b->max.x && a->max.x >= b->min.x) && (a->min.y <= b->max.y && a->max.y >= b->min.y) && (a->min.z <= b->max.z && a->max.z >= b->min.z);
#endif
}

RGINLINE void rg_aabb_closest_point(const rg_aabb* aabb, const rg_vec3* p, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_min_ps(_mm_max_ps(RG_VEC3_LOAD(p), RG_VEC3_LOAD(&aabb->min)), RG_VEC3_LOAD(&aabb->max)));
#else
	out->x = rg_clampf(p->x, aabb->min.x, aabb->max.x);
	out->y = rg_clampf(p->y, aabb->min.y, aabb->max.y);
	out->z = rg_clampf(p->z, aabb->min.z, aabb->max.z);
#endif
}

// =============================================================================
// AABB2D HELPERS
// =============================================================================

RGINLINE void rg_aabb2_set(rg_aabb2* aabb, const rg_vec2* min, const rg_vec2* max)
{
	aabb->min = *min;
	aabb->max = *max;
}

RGINLINE void rg_aabb2_zero(rg_aabb2* aabb)
{
	aabb->min.x = 0.0f;
	aabb->min.y = 0.0f;
	aabb->max.x = 0.0f;
	aabb->max.y = 0.0f;
}

RGINLINE void rg_aabb2_copy(const rg_aabb2* aabb, rg_aabb2* out)
{
	*out = *aabb;
}

RGINLINE void rg_aabb2_transform(const rg_aabb2* aabb, const rg_mat3* m, rg_aabb2* out)
{
	f32 x0 = m->m[0] * aabb->min.x;
	f32 x1 = m->m[0] * aabb->max.x;
	f32 y0 = m->m[4] * aabb->min.y;
	f32 y1 = m->m[4] * aabb->max.y;

	f32 x2 = m->m[1] * aabb->min.x;
	f32 x3 = m->m[1] * aabb->max.x;
	f32 y2 = m->m[5] * aabb->min.y;
	f32 y3 = m->m[5] * aabb->max.y;

	out->min.x = m->m[8] + rg_minf(x0, x1) + rg_minf(y0, y1);
	out->min.y = m->m[9] + rg_minf(x2, x3) + rg_minf(y2, y3);
	out->max.x = m->m[8] + rg_maxf(x0, x1) + rg_maxf(y0, y1);
	out->max.y = m->m[9] + rg_maxf(x2, x3) + rg_maxf(y2, y3);
}

RGINLINE void rg_aabb2_merge(const rg_aabb2* a, const rg_aabb2* b, rg_aabb2* out)
{
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	__m128 av = _mm_loadu_ps(&a->min.x);
	__m128 bv = _mm_loadu_ps(&b->min.x);
	_mm_storeu_ps(&out->min.x, _mm_blend_ps(_mm_min_ps(av, bv), _mm_max_ps(av, bv), 0xC));
#else
	out->min.x = rg_minf(a->min.x, b->min.x);
	out->min.y = rg_minf(a->min.y, b->min.y);
	out->max.x = rg_maxf(a->max.x, b->max.x);
	out->max.y = rg_maxf(a->max.y, b->max.y);
#endif
}

RGINLINE void rg_aabb2_crop(const rg_aabb2* aabb, const rg_aabb2* crop, rg_aabb2* out)
{
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	__m128 av = _mm_loadu_ps(&aabb->min.x);
	__m128 cv = _mm_loadu_ps(&crop->min.x);
	_mm_storeu_ps(&out->min.x, _mm_blend_ps(_mm_max_ps(av, cv), _mm_min_ps(av, cv), 0xC));
#else
	out->min.x = rg_maxf(aabb->min.x, crop->min.x);
	out->min.y = rg_maxf(aabb->min.y, crop->min.y);
	out->max.x = rg_minf(aabb->max.x, crop->max.x);
	out->max.y = rg_minf(aabb->max.y, crop->max.y);
#endif
}

RGINLINE void rg_aabb2_crop_until(const rg_aabb2* aabb, const rg_aabb2* crop, const rg_aabb2* clamp, rg_aabb2* out)
{
	rg_aabb2_crop(aabb, crop, out);
	rg_aabb2_merge(clamp, out, out);
}

RGINLINE void rg_aabb2_invalidate(rg_aabb2* aabb)
{
	aabb->min.x = FLT_MAX;
	aabb->min.y = FLT_MAX;
	aabb->max.x = -FLT_MAX;
	aabb->max.y = -FLT_MAX;
}

RGINLINE int rg_aabb2_isvalid(const rg_aabb2* aabb)
{
	return rg_maxf(aabb->min.x, aabb->min.y) != FLT_MAX && rg_minf(aabb->max.x, aabb->max.y) != -FLT_MAX;
}

RGINLINE f32 rg_aabb2_diag(const rg_aabb2* aabb)
{
	return rg_vec2_distance(&aabb->min, &aabb->max);
}

RGINLINE void rg_aabb2_size(const rg_aabb2* aabb, rg_vec2* out)
{
	out->x = aabb->max.x - aabb->min.x;
	out->y = aabb->max.y - aabb->min.y;
}

RGINLINE f32 rg_aabb2_radius(const rg_aabb2* aabb)
{
	f32 dx = aabb->max.x - aabb->min.x;
	f32 dy = aabb->max.y - aabb->min.y;
	return rg_sqrtf(dx * dx + dy * dy) * 0.5f;
}

RGINLINE void rg_aabb2_center(const rg_aabb2* aabb, rg_vec2* out)
{
	out->x = (aabb->min.x + aabb->max.x) * 0.5f;
	out->y = (aabb->min.y + aabb->max.y) * 0.5f;
}

RGINLINE int rg_aabb2_overlaps(const rg_aabb2* aabb, const rg_aabb2* other)
{
	return (aabb->min.x <= other->max.x && aabb->max.x >= other->min.x) && (aabb->min.y <= other->max.y && aabb->max.y >= other->min.y);
}

RGINLINE int rg_aabb2_circle(const rg_aabb2* aabb, const rg_circle* circle)
{
	f32 dx = 0.0f;
	f32 dy = 0.0f;

	if (circle->center.x < aabb->min.x)
	{
		dx = aabb->min.x - circle->center.x;
	}
	else if (circle->center.x > aabb->max.x)
	{
		dx = circle->center.x - aabb->max.x;
	}

	if (circle->center.y < aabb->min.y)
	{
		dy = aabb->min.y - circle->center.y;
	}
	else if (circle->center.y > aabb->max.y)
	{
		dy = circle->center.y - aabb->max.y;
	}

	return (dx * dx + dy * dy) <= circle->radius * circle->radius;
}

RGINLINE int rg_aabb2_contains_point(const rg_aabb2* aabb, const rg_vec2* p)
{
	return (p->x >= aabb->min.x && p->x <= aabb->max.x) && (p->y >= aabb->min.y && p->y <= aabb->max.y);
}

RGINLINE int rg_aabb2_contains(const rg_aabb2* aabb, const rg_aabb2* other)
{
	return (aabb->min.x <= other->min.x && aabb->max.x >= other->max.x) && (aabb->min.y <= other->min.y && aabb->max.y >= other->max.y);
}

RGINLINE void rg_sphere_set(rg_sphere* sphere, const rg_vec3* center, f32 radius)
{
	sphere->center = *center;
	sphere->radius = radius;
}

RGINLINE void rg_sphere_transform(const rg_sphere* sphere, const rg_mat4* m, rg_sphere* out)
{
	f32 x = sphere->center.x;
	f32 y = sphere->center.y;
	f32 z = sphere->center.z;
	f32 radius = sphere->radius;

	out->center.x = m->m[0] * x + m->m[4] * y + m->m[8] * z + m->m[12];
	out->center.y = m->m[1] * x + m->m[5] * y + m->m[9] * z + m->m[13];
	out->center.z = m->m[2] * x + m->m[6] * y + m->m[10] * z + m->m[14];
#if !RG_MATH_MAX_PERF
	out->center._pad = 0.0f;
#endif
	out->radius = radius;
}

RGINLINE void rg_sphere_merge(const rg_sphere* a, const rg_sphere* b, rg_sphere* out)
{
	f32 dx = b->center.x - a->center.x;
	f32 dy = b->center.y - a->center.y;
	f32 dz = b->center.z - a->center.z;
	f32 dist_sq = dx * dx + dy * dy + dz * dz;
	f32 dist = rg_sqrtf(dist_sq);

	if (dist <= RG_EPSILON)
	{
		if (a->radius >= b->radius)
		{
			*out = *a;
		}
		else
		{
			*out = *b;
		}
		return;
	}

	if (a->radius >= dist + b->radius)
	{
		*out = *a;
		return;
	}

	if (b->radius >= dist + a->radius)
	{
		*out = *b;
		return;
	}

	{
		f32 radius = 0.5f * (dist + a->radius + b->radius);
		f32 t = (radius - a->radius) / dist;
		out->center.x = a->center.x + dx * t;
		out->center.y = a->center.y + dy * t;
		out->center.z = a->center.z + dz * t;
#if !RG_MATH_MAX_PERF
		out->center._pad = 0.0f;
#endif
		out->radius = radius;
	}
}

RGINLINE int rg_sphere_contains_point(const rg_sphere* sphere, const rg_vec3* p)
{
	f32 dx = p->x - sphere->center.x;
	f32 dy = p->y - sphere->center.y;
	f32 dz = p->z - sphere->center.z;
	f32 dist2 = dx * dx + dy * dy + dz * dz;
	return dist2 <= sphere->radius * sphere->radius;
}

RGINLINE int rg_sphere_overlaps(const rg_sphere* a, const rg_sphere* b)
{
	f32 dx = b->center.x - a->center.x;
	f32 dy = b->center.y - a->center.y;
	f32 dz = b->center.z - a->center.z;
	f32 r = a->radius + b->radius;
	f32 dist2 = dx * dx + dy * dy + dz * dz;
	return dist2 <= r * r;
}

RGINLINE void rg_plane_from_point_normal(rg_plane* plane, const rg_vec3* point, const rg_vec3* normal)
{
	f32 nx = normal->x;
	f32 ny = normal->y;
	f32 nz = normal->z;
	plane->normal.x = nx;
	plane->normal.y = ny;
	plane->normal.z = nz;
	plane->d = -(nx * point->x + ny * point->y + nz * point->z);
}

RGINLINE void rg_plane_from_triangle(rg_plane* plane, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c)
{
#if RG_MATH_MAX_PERF
	f32 e1x = b->x - a->x;
	f32 e1y = b->y - a->y;
	f32 e1z = b->z - a->z;
	f32 e2x = c->x - a->x;
	f32 e2y = c->y - a->y;
	f32 e2z = c->z - a->z;

	f32 nx = e1y * e2z - e1z * e2y;
	f32 ny = e1z * e2x - e1x * e2z;
	f32 nz = e1x * e2y - e1y * e2x;
	f32 len_sq = nx * nx + ny * ny + nz * nz;
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	nx *= inv_len;
	ny *= inv_len;
	nz *= inv_len;

	plane->normal.x = nx;
	plane->normal.y = ny;
	plane->normal.z = nz;
	plane->d = -(nx * a->x + ny * a->y + nz * a->z);
#else
	rg_vec3 edge1;
	rg_vec3 edge2;
	rg_vec3_sub(b, a, &edge1);
	rg_vec3_sub(c, a, &edge2);
	rg_vec3_cross(&edge1, &edge2, &plane->normal);
	rg_vec3_normalize(&plane->normal, &plane->normal);
	plane->d = -rg_vec3_dot(&plane->normal, a);
#endif
}

RGINLINE void rg_plane_normalize(rg_plane* plane)
{
	f32 len_sq = plane->normal.x * plane->normal.x + plane->normal.y * plane->normal.y + plane->normal.z * plane->normal.z;

	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		plane->normal.x = 0.0f;
		plane->normal.y = 0.0f;
		plane->normal.z = 0.0f;
		plane->d = 0.0f;
		return;
	}

#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	plane->normal.x *= inv_len;
	plane->normal.y *= inv_len;
	plane->normal.z *= inv_len;
	plane->d *= inv_len;
}

RGINLINE f32 rg_plane_distance(const rg_plane* plane, const rg_vec3* p)
{
	return rg_vec3_dot(&plane->normal, p) + plane->d;
}

RGINLINE void rg_plane_project_point(const rg_plane* plane, const rg_vec3* p, rg_vec3* out)
{
	f32 nx = plane->normal.x;
	f32 ny = plane->normal.y;
	f32 nz = plane->normal.z;
	f32 dist = nx * p->x + ny * p->y + nz * p->z + plane->d;
	out->x = p->x - dist * nx;
	out->y = p->y - dist * ny;
	out->z = p->z - dist * nz;
}

RGINLINE void rg_frustum_from_mat4(const rg_mat4* m, rg_frustum* out)
{
	const f32* a = m->m;
	rg_plane* planes = out->planes;

#ifdef RG_MATH_FAST_NORMALIZE
#define RG_FRUSTUM_INV_LEN(len_sq) rg_rsqrtf_fast((len_sq))
#else
#define RG_FRUSTUM_INV_LEN(len_sq) rg_rsqrtf((len_sq))
#endif

#define RG_FRUSTUM_SET_NORMALIZED(index, px, py, pz, pd) \
	do                                                   \
	{                                                    \
		f32 nx = (px);                                   \
		f32 ny = (py);                                   \
		f32 nz = (pz);                                   \
		f32 nd = (pd);                                   \
		f32 len_sq = nx * nx + ny * ny + nz * nz;        \
		if (len_sq < RG_EPSILON * RG_EPSILON)            \
		{                                                \
			planes[(index)].normal.x = 0.0f;             \
			planes[(index)].normal.y = 0.0f;             \
			planes[(index)].normal.z = 0.0f;             \
			planes[(index)].d = 0.0f;                    \
		}                                                \
		else                                             \
		{                                                \
			f32 inv_len = RG_FRUSTUM_INV_LEN(len_sq);    \
			planes[(index)].normal.x = nx * inv_len;     \
			planes[(index)].normal.y = ny * inv_len;     \
			planes[(index)].normal.z = nz * inv_len;     \
			planes[(index)].d = nd * inv_len;            \
		}                                                \
	} while (0)

	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_LEFT, a[3] + a[0], a[7] + a[4], a[11] + a[8], a[15] + a[12]);
	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_RIGHT, a[3] - a[0], a[7] - a[4], a[11] - a[8], a[15] - a[12]);
	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_BOTTOM, a[3] + a[1], a[7] + a[5], a[11] + a[9], a[15] + a[13]);
	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_TOP, a[3] - a[1], a[7] - a[5], a[11] - a[9], a[15] - a[13]);
	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_NEAR, a[3] + a[2], a[7] + a[6], a[11] + a[10], a[15] + a[14]);
	RG_FRUSTUM_SET_NORMALIZED(RG_FRUSTUM_FAR, a[3] - a[2], a[7] - a[6], a[11] - a[10], a[15] - a[14]);

#undef RG_FRUSTUM_SET_NORMALIZED
#undef RG_FRUSTUM_INV_LEN
}

RGINLINE int rg_aabb_in_frustum(const rg_aabb* aabb, const rg_frustum* frustum)
{
	const f32 minx = aabb->min.x;
	const f32 miny = aabb->min.y;
	const f32 minz = aabb->min.z;
	const f32 maxx = aabb->max.x;
	const f32 maxy = aabb->max.y;
	const f32 maxz = aabb->max.z;

	const rg_plane* plane = frustum->planes;
	for (int i = 0; i < RG_FRUSTUM_PLANE_COUNT; i++, plane++)
	{
		const f32 nx = plane->normal.x;
		const f32 ny = plane->normal.y;
		const f32 nz = plane->normal.z;
		const f32 dp = nx * (nx > 0.0f ? maxx : minx) + ny * (ny > 0.0f ? maxy : miny) + nz * (nz > 0.0f ? maxz : minz) + plane->d;
		if (RG_UNLIKELY(dp < 0.0f))
		{
			return 0;
		}
	}

	return 1;
}

RGINLINE void rg_frustum_corners(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT])
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_ZO_BIT)
	rg_frustum_corners_zo(inv_mat, out);
#else
	rg_frustum_corners_no(inv_mat, out);
#endif
}

RGINLINE void rg_frustum_corner_from_clip(const f32* m, f32 cx, f32 cy, f32 cz, rg_vec4* out)
{
	f32 x = m[0] * cx + m[4] * cy + m[8] * cz + m[12];
	f32 y = m[1] * cx + m[5] * cy + m[9] * cz + m[13];
	f32 z = m[2] * cx + m[6] * cy + m[10] * cz + m[14];
	f32 w = m[3] * cx + m[7] * cy + m[11] * cz + m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(w) < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 1.0f;
		return;
	}
#endif
	f32 inv_w = 1.0f / w;
	out->x = x * inv_w;
	out->y = y * inv_w;
	out->z = z * inv_w;
	out->w = 1.0f;
}

RGINLINE void rg_frustum_corners_no(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT])
{
	const f32* m = inv_mat->m;
	rg_frustum_corner_from_clip(m, -1.0f, -1.0f, -1.0f, &out[RG_FRUSTUM_CORNER_LBN]);
	rg_frustum_corner_from_clip(m, -1.0f, 1.0f, -1.0f, &out[RG_FRUSTUM_CORNER_LTN]);
	rg_frustum_corner_from_clip(m, 1.0f, 1.0f, -1.0f, &out[RG_FRUSTUM_CORNER_RTN]);
	rg_frustum_corner_from_clip(m, 1.0f, -1.0f, -1.0f, &out[RG_FRUSTUM_CORNER_RBN]);
	rg_frustum_corner_from_clip(m, -1.0f, -1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_LBF]);
	rg_frustum_corner_from_clip(m, -1.0f, 1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_LTF]);
	rg_frustum_corner_from_clip(m, 1.0f, 1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_RTF]);
	rg_frustum_corner_from_clip(m, 1.0f, -1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_RBF]);
}

RGINLINE void rg_frustum_corners_zo(const rg_mat4* inv_mat, rg_vec4 out[RG_FRUSTUM_CORNER_COUNT])
{
	const f32* m = inv_mat->m;
	rg_frustum_corner_from_clip(m, -1.0f, -1.0f, 0.0f, &out[RG_FRUSTUM_CORNER_LBN]);
	rg_frustum_corner_from_clip(m, -1.0f, 1.0f, 0.0f, &out[RG_FRUSTUM_CORNER_LTN]);
	rg_frustum_corner_from_clip(m, 1.0f, 1.0f, 0.0f, &out[RG_FRUSTUM_CORNER_RTN]);
	rg_frustum_corner_from_clip(m, 1.0f, -1.0f, 0.0f, &out[RG_FRUSTUM_CORNER_RBN]);
	rg_frustum_corner_from_clip(m, -1.0f, -1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_LBF]);
	rg_frustum_corner_from_clip(m, -1.0f, 1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_LTF]);
	rg_frustum_corner_from_clip(m, 1.0f, 1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_RTF]);
	rg_frustum_corner_from_clip(m, 1.0f, -1.0f, 1.0f, &out[RG_FRUSTUM_CORNER_RBF]);
}

RGINLINE void rg_frustum_center(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 sum = RG_VEC4_LOAD(&corners[0]);
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[1]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[2]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[3]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[4]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[5]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[6]));
	sum = _mm_add_ps(sum, RG_VEC4_LOAD(&corners[7]));
	RG_VEC4_STORE(out, _mm_mul_ps(sum, _mm_set1_ps(0.125f)));
#else
	f32 x = corners[0].x;
	f32 y = corners[0].y;
	f32 z = corners[0].z;
	f32 w = corners[0].w;
	for (int i = 1; i < RG_FRUSTUM_CORNER_COUNT; i++)
	{
		x += corners[i].x;
		y += corners[i].y;
		z += corners[i].z;
		w += corners[i].w;
	}
	f32 inv = 1.0f / (f32)RG_FRUSTUM_CORNER_COUNT;
	out->x = x * inv;
	out->y = y * inv;
	out->z = z * inv;
	out->w = w * inv;
#endif
}

RGINLINE void rg_frustum_box(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], const rg_mat4* m, rg_aabb* out)
{
	rg_vec4 v;
	rg_mat4_mulv4(m, &corners[0], &v);
	f32 min_x = v.x;
	f32 min_y = v.y;
	f32 min_z = v.z;
	f32 max_x = v.x;
	f32 max_y = v.y;
	f32 max_z = v.z;

	for (int i = 1; i < RG_FRUSTUM_CORNER_COUNT; i++)
	{
		rg_mat4_mulv4(m, &corners[i], &v);
		min_x = rg_minf(min_x, v.x);
		min_y = rg_minf(min_y, v.y);
		min_z = rg_minf(min_z, v.z);
		max_x = rg_maxf(max_x, v.x);
		max_y = rg_maxf(max_y, v.y);
		max_z = rg_maxf(max_z, v.z);
	}

	out->min.x = min_x;
	out->min.y = min_y;
	out->min.z = min_z;
	out->max.x = max_x;
	out->max.y = max_y;
	out->max.z = max_z;
#if !RG_MATH_MAX_PERF
	out->min._pad = 0.0f;
	out->max._pad = 0.0f;
#endif
}

RGINLINE void rg_frustum_corners_at(const rg_vec4 corners[RG_FRUSTUM_CORNER_COUNT], f32 split_dist, f32 far_dist, rg_vec4 out[4])
{
	RG_MATH_ASSERT(far_dist != 0.0f);
	f32 t = split_dist / far_dist;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 tv = _mm_set1_ps(t);
	__m128 n0 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_LBN]);
	__m128 n1 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_LTN]);
	__m128 n2 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_RTN]);
	__m128 n3 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_RBN]);
	__m128 f0 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_LBF]);
	__m128 f1 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_LTF]);
	__m128 f2 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_RTF]);
	__m128 f3 = RG_VEC4_LOAD(&corners[RG_FRUSTUM_CORNER_RBF]);
	RG_VEC4_STORE(&out[0], RG_MATH_FMADD_PS(_mm_sub_ps(f0, n0), tv, n0));
	RG_VEC4_STORE(&out[1], RG_MATH_FMADD_PS(_mm_sub_ps(f1, n1), tv, n1));
	RG_VEC4_STORE(&out[2], RG_MATH_FMADD_PS(_mm_sub_ps(f2, n2), tv, n2));
	RG_VEC4_STORE(&out[3], RG_MATH_FMADD_PS(_mm_sub_ps(f3, n3), tv, n3));
#else
	const rg_vec4* near_lbn = &corners[RG_FRUSTUM_CORNER_LBN];
	const rg_vec4* near_ltn = &corners[RG_FRUSTUM_CORNER_LTN];
	const rg_vec4* near_rtn = &corners[RG_FRUSTUM_CORNER_RTN];
	const rg_vec4* near_rbn = &corners[RG_FRUSTUM_CORNER_RBN];

	const rg_vec4* far_lbf = &corners[RG_FRUSTUM_CORNER_LBF];
	const rg_vec4* far_ltf = &corners[RG_FRUSTUM_CORNER_LTF];
	const rg_vec4* far_rtf = &corners[RG_FRUSTUM_CORNER_RTF];
	const rg_vec4* far_rbf = &corners[RG_FRUSTUM_CORNER_RBF];

	out[0].x = near_lbn->x + (far_lbf->x - near_lbn->x) * t;
	out[0].y = near_lbn->y + (far_lbf->y - near_lbn->y) * t;
	out[0].z = near_lbn->z + (far_lbf->z - near_lbn->z) * t;
	out[0].w = near_lbn->w + (far_lbf->w - near_lbn->w) * t;

	out[1].x = near_ltn->x + (far_ltf->x - near_ltn->x) * t;
	out[1].y = near_ltn->y + (far_ltf->y - near_ltn->y) * t;
	out[1].z = near_ltn->z + (far_ltf->z - near_ltn->z) * t;
	out[1].w = near_ltn->w + (far_ltf->w - near_ltn->w) * t;

	out[2].x = near_rtn->x + (far_rtf->x - near_rtn->x) * t;
	out[2].y = near_rtn->y + (far_rtf->y - near_rtn->y) * t;
	out[2].z = near_rtn->z + (far_rtf->z - near_rtn->z) * t;
	out[2].w = near_rtn->w + (far_rtf->w - near_rtn->w) * t;

	out[3].x = near_rbn->x + (far_rbf->x - near_rbn->x) * t;
	out[3].y = near_rbn->y + (far_rbf->y - near_rbn->y) * t;
	out[3].z = near_rbn->z + (far_rbf->z - near_rbn->z) * t;
	out[3].w = near_rbn->w + (far_rbf->w - near_rbn->w) * t;
#endif
}

RGINLINE int rg_ray_plane(const rg_ray* ray, const rg_plane* plane, f32* t)
{
	f32 nx = plane->normal.x;
	f32 ny = plane->normal.y;
	f32 nz = plane->normal.z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 denom = nx * dx + ny * dy + nz * dz;

#if RG_MATH_MAX_PERF
	if (RG_UNLIKELY(denom > -RG_EPSILON && denom < RG_EPSILON))
	{
		return 0;
	}
#else
	if (rg_absf(denom) < RG_EPSILON)
	{
		return 0;
	}
#endif

	f32 ox = ray->origin.x;
	f32 oy = ray->origin.y;
	f32 oz = ray->origin.z;
	f32 num = nx * ox + ny * oy + nz * oz + plane->d;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 inv = _mm_rcp_ss(_mm_set_ss(denom));
	f32 t_hit = -num * _mm_cvtss_f32(inv);
#else
	f32 t_hit = -num / denom;
#endif
	if (t)
	{
		*t = t_hit;
	}
	return t_hit >= 0.0f;
}

RGINLINE int rg_ray_plane_fast(const rg_ray* ray, const rg_plane* plane, f32* t)
{
	f32 nx = plane->normal.x;
	f32 ny = plane->normal.y;
	f32 nz = plane->normal.z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 denom = nx * dx + ny * dy + nz * dz;

	f32 ox = ray->origin.x;
	f32 oy = ray->origin.y;
	f32 oz = ray->origin.z;
	f32 num = nx * ox + ny * oy + nz * oz + plane->d;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 inv = _mm_rcp_ss(_mm_set_ss(denom));
	f32 t_hit = -num * _mm_cvtss_f32(inv);
#else
	f32 t_hit = -num / denom;
#endif
	if (t)
	{
		*t = t_hit;
	}
	return t_hit >= 0.0f;
}

RGINLINE int rg_ray_plane_point_normal(const rg_ray* ray, const rg_vec3* point, const rg_vec3* normal, f32* t)
{
	f32 nx = normal->x;
	f32 ny = normal->y;
	f32 nz = normal->z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 denom = nx * dx + ny * dy + nz * dz;

#if RG_MATH_MAX_PERF
	if (RG_UNLIKELY(denom > -RG_EPSILON && denom < RG_EPSILON))
	{
		return 0;
	}
#else
	if (rg_absf(denom) < RG_EPSILON)
	{
		return 0;
	}
#endif

	f32 ox = ray->origin.x;
	f32 oy = ray->origin.y;
	f32 oz = ray->origin.z;
	f32 px = point->x - ox;
	f32 py = point->y - oy;
	f32 pz = point->z - oz;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 inv = _mm_rcp_ss(_mm_set_ss(denom));
	f32 t_hit = (px * nx + py * ny + pz * nz) * _mm_cvtss_f32(inv);
#else
	f32 t_hit = (px * nx + py * ny + pz * nz) / denom;
#endif
	if (t)
	{
		*t = t_hit;
	}
	return t_hit >= 0.0f;
}

RGINLINE int rg_ray_plane_point_normal_fast(const rg_ray* ray, const rg_vec3* point, const rg_vec3* normal, f32* t)
{
	f32 nx = normal->x;
	f32 ny = normal->y;
	f32 nz = normal->z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 denom = nx * dx + ny * dy + nz * dz;

	f32 ox = ray->origin.x;
	f32 oy = ray->origin.y;
	f32 oz = ray->origin.z;
	f32 px = point->x - ox;
	f32 py = point->y - oy;
	f32 pz = point->z - oz;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 inv = _mm_rcp_ss(_mm_set_ss(denom));
	f32 t_hit = (px * nx + py * ny + pz * nz) * _mm_cvtss_f32(inv);
#else
	f32 t_hit = (px * nx + py * ny + pz * nz) / denom;
#endif
	if (t)
	{
		*t = t_hit;
	}
	return t_hit >= 0.0f;
}

RGINLINE int rg_ray_sphere(const rg_ray* ray, const rg_sphere* sphere, f32* t1, f32* t2)
{
	f32 dx = sphere->center.x - ray->origin.x;
	f32 dy = sphere->center.y - ray->origin.y;
	f32 dz = sphere->center.z - ray->origin.z;

	f32 ddp = ray->dir.x * dx + ray->dir.y * dy + ray->dir.z * dz;
	f32 dpp = dx * dx + dy * dy + dz * dz;

	f32 rx = dx - ray->dir.x * ddp;
	f32 ry = dy - ray->dir.y * ddp;
	f32 rz = dz - ray->dir.z * ddp;

	f32 r2 = sphere->radius * sphere->radius;
	f32 dscr = r2 - (rx * rx + ry * ry + rz * rz);

	if (dscr < 0.0f)
	{
		return 0;
	}

	f32 root = rg_sqrtf(dscr);
	f32 q = (ddp >= 0.0f) ? (ddp + root) : (ddp - root);
	f32 _t1 = q;
	f32 _t2 = (dpp - r2) / q;

	if (_t1 > _t2)
	{
		f32 tmp = _t1;
		_t1 = _t2;
		_t2 = tmp;
	}

	if (t1) *t1 = _t1;
	if (t2) *t2 = _t2;

	if (_t1 < 0.0f && _t2 < 0.0f)
	{
		return 0;
	}

	return 1;
}

RGINLINE int rg_ray_sphere_distance(const rg_ray* ray, const rg_sphere* sphere, f32* t)
{
	f32 dx = sphere->center.x - ray->origin.x;
	f32 dy = sphere->center.y - ray->origin.y;
	f32 dz = sphere->center.z - ray->origin.z;

	f32 t0 = dx * ray->dir.x + dy * ray->dir.y + dz * ray->dir.z;
	f32 d2 = dx * dx + dy * dy + dz * dz - t0 * t0;
	f32 r2 = sphere->radius * sphere->radius;

	if (d2 > r2)
	{
		return 0;
	}

	f32 t1 = rg_sqrtf(r2 - d2);
	f32 dist = (t0 > t1 + RG_EPSILON) ? (t0 - t1) : (t0 + t1);

	if (t)
	{
		*t = dist;
	}

	return dist > RG_EPSILON;
}

RGINLINE int rg_ray_triangle(const rg_ray* ray, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, f32* t)
{
	const f32 epsilon = 0.000001f;
	f32 e1x = b->x - a->x;
	f32 e1y = b->y - a->y;
	f32 e1z = b->z - a->z;
	f32 e2x = c->x - a->x;
	f32 e2y = c->y - a->y;
	f32 e2z = c->z - a->z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 px = dy * e2z - dz * e2y;
	f32 py = dz * e2x - dx * e2z;
	f32 pz = dx * e2y - dy * e2x;

	f32 det = e1x * px + e1y * py + e1z * pz;
	if (det > -epsilon && det < epsilon)
	{
		return 0;
	}

	f32 inv_det = 1.0f / det;
	f32 tx = ray->origin.x - a->x;
	f32 ty = ray->origin.y - a->y;
	f32 tz = ray->origin.z - a->z;

	f32 u = inv_det * (tx * px + ty * py + tz * pz);
	if (u < 0.0f || u > 1.0f)
	{
		return 0;
	}

	f32 qx = ty * e1z - tz * e1y;
	f32 qy = tz * e1x - tx * e1z;
	f32 qz = tx * e1y - ty * e1x;
	f32 v = inv_det * (dx * qx + dy * qy + dz * qz);
	if (v < 0.0f || (u + v) > 1.0f)
	{
		return 0;
	}

	f32 dist = inv_det * (e2x * qx + e2y * qy + e2z * qz);
	if (t)
	{
		*t = dist;
	}

	return dist > epsilon;
}

RGINLINE int rg_ray_triangle_bary(const rg_ray* ray, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, rg_vec2* bary, f32* t)
{
	const f32 epsilon = 0.000001f;
	f32 e1x = b->x - a->x;
	f32 e1y = b->y - a->y;
	f32 e1z = b->z - a->z;
	f32 e2x = c->x - a->x;
	f32 e2y = c->y - a->y;
	f32 e2z = c->z - a->z;
	f32 dx = ray->dir.x;
	f32 dy = ray->dir.y;
	f32 dz = ray->dir.z;
	f32 px = dy * e2z - dz * e2y;
	f32 py = dz * e2x - dx * e2z;
	f32 pz = dx * e2y - dy * e2x;

	f32 det = e1x * px + e1y * py + e1z * pz;
	if (det > -epsilon && det < epsilon)
	{
		return 0;
	}

	f32 inv_det = 1.0f / det;
	f32 tx = ray->origin.x - a->x;
	f32 ty = ray->origin.y - a->y;
	f32 tz = ray->origin.z - a->z;

	f32 u = inv_det * (tx * px + ty * py + tz * pz);
	if (u < 0.0f || u > 1.0f)
	{
		return 0;
	}

	f32 qx = ty * e1z - tz * e1y;
	f32 qy = tz * e1x - tx * e1z;
	f32 qz = tx * e1y - ty * e1x;
	f32 v = inv_det * (dx * qx + dy * qy + dz * qz);
	if (v < 0.0f || (u + v) > 1.0f)
	{
		return 0;
	}

	f32 dist = inv_det * (e2x * qx + e2y * qy + e2z * qz);

	if (bary)
	{
		bary->x = u;
		bary->y = v;
	}
	if (t)
	{
		*t = dist;
	}

	return dist > epsilon;
}

RGINLINE int rg_line_triangle(const rg_vec3* origin, const rg_vec3* dir, const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, rg_vec2* bary, f32* t)
{
	const f32 epsilon = 0.000001f;
	f32 e1x = b->x - a->x;
	f32 e1y = b->y - a->y;
	f32 e1z = b->z - a->z;
	f32 e2x = c->x - a->x;
	f32 e2y = c->y - a->y;
	f32 e2z = c->z - a->z;
	f32 dx = dir->x;
	f32 dy = dir->y;
	f32 dz = dir->z;
	f32 px = dy * e2z - dz * e2y;
	f32 py = dz * e2x - dx * e2z;
	f32 pz = dx * e2y - dy * e2x;

	f32 det = e1x * px + e1y * py + e1z * pz;
	if (det > -epsilon && det < epsilon)
	{
		return 0;
	}

	f32 inv_det = 1.0f / det;
	f32 tx = origin->x - a->x;
	f32 ty = origin->y - a->y;
	f32 tz = origin->z - a->z;

	f32 u = inv_det * (tx * px + ty * py + tz * pz);
	if (u < 0.0f || u > 1.0f)
	{
		return 0;
	}

	f32 qx = ty * e1z - tz * e1y;
	f32 qy = tz * e1x - tx * e1z;
	f32 qz = tx * e1y - ty * e1x;
	f32 v = inv_det * (dx * qx + dy * qy + dz * qz);
	if (v < 0.0f || (u + v) > 1.0f)
	{
		return 0;
	}

	f32 dist = inv_det * (e2x * qx + e2y * qy + e2z * qz);

	if (bary)
	{
		bary->x = u;
		bary->y = v;
	}
	if (t)
	{
		*t = dist;
	}

	return 1;
}

RGINLINE int rg_line_sphere(const rg_vec3* p0, const rg_vec3* p1, const rg_sphere* sphere, rg_vec3* out_p1, rg_vec3* out_n1, rg_vec3* out_p2, rg_vec3* out_n2)
{
	f32 dirx = p1->x - p0->x;
	f32 diry = p1->y - p0->y;
	f32 dirz = p1->z - p0->z;
	f32 a = dirx * dirx + diry * diry + dirz * dirz;

	if (a < RG_EPSILON * RG_EPSILON)
	{
		return 0;
	}

	f32 mx = p0->x - sphere->center.x;
	f32 my = p0->y - sphere->center.y;
	f32 mz = p0->z - sphere->center.z;
	f32 r = sphere->radius;
	f32 c = mx * mx + my * my + mz * mz - r * r;
	f32 b = 2.0f * (mx * dirx + my * diry + mz * dirz);
	f32 disc = b * b - 4.0f * a * c;

	if (disc < 0.0f)
	{
		return 0;
	}

	f32 sqrt_disc = rg_sqrtf(disc);
	f32 inv_2a = 0.5f / a;
	f32 hit1 = (-b - sqrt_disc) * inv_2a;
	f32 hit2 = (-b + sqrt_disc) * inv_2a;
	f32 inv_r = 1.0f / r;

	f32 p1x = p0->x + hit1 * dirx;
	f32 p1y = p0->y + hit1 * diry;
	f32 p1z = p0->z + hit1 * dirz;
	f32 p2x = p0->x + hit2 * dirx;
	f32 p2y = p0->y + hit2 * diry;
	f32 p2z = p0->z + hit2 * dirz;

	if (out_p1)
	{
		out_p1->x = p1x;
		out_p1->y = p1y;
		out_p1->z = p1z;
#if !RG_MATH_MAX_PERF
		out_p1->_pad = 0.0f;
#endif
	}
	if (out_n1)
	{
		out_n1->x = (p1x - sphere->center.x) * inv_r;
		out_n1->y = (p1y - sphere->center.y) * inv_r;
		out_n1->z = (p1z - sphere->center.z) * inv_r;
#if !RG_MATH_MAX_PERF
		out_n1->_pad = 0.0f;
#endif
	}
	if (out_p2)
	{
		out_p2->x = p2x;
		out_p2->y = p2y;
		out_p2->z = p2z;
#if !RG_MATH_MAX_PERF
		out_p2->_pad = 0.0f;
#endif
	}
	if (out_n2)
	{
		out_n2->x = (p2x - sphere->center.x) * inv_r;
		out_n2->y = (p2y - sphere->center.y) * inv_r;
		out_n2->z = (p2z - sphere->center.z) * inv_r;
#if !RG_MATH_MAX_PERF
		out_n2->_pad = 0.0f;
#endif
	}

	return 1;
}

RGINLINE int rg_ray_aabb(const rg_ray* ray, const rg_aabb* aabb, f32* tmin, f32* tmax)
{
	f32 t0 = -FLT_MAX;
	f32 t1 = FLT_MAX;

	if (rg_absf(ray->dir.x) < RG_EPSILON)
	{
		if (ray->origin.x < aabb->min.x || ray->origin.x > aabb->max.x)
		{
			return 0;
		}
	}
	else
	{
		f32 inv = 1.0f / ray->dir.x;
		f32 tminx = (aabb->min.x - ray->origin.x) * inv;
		f32 tmaxx = (aabb->max.x - ray->origin.x) * inv;
		if (tminx > tmaxx)
		{
			f32 tmp = tminx;
			tminx = tmaxx;
			tmaxx = tmp;
		}
		t0 = rg_maxf(t0, tminx);
		t1 = rg_minf(t1, tmaxx);
		if (t0 > t1)
		{
			return 0;
		}
	}

	if (rg_absf(ray->dir.y) < RG_EPSILON)
	{
		if (ray->origin.y < aabb->min.y || ray->origin.y > aabb->max.y)
		{
			return 0;
		}
	}
	else
	{
		f32 inv = 1.0f / ray->dir.y;
		f32 tminy = (aabb->min.y - ray->origin.y) * inv;
		f32 tmaxy = (aabb->max.y - ray->origin.y) * inv;
		if (tminy > tmaxy)
		{
			f32 tmp = tminy;
			tminy = tmaxy;
			tmaxy = tmp;
		}
		t0 = rg_maxf(t0, tminy);
		t1 = rg_minf(t1, tmaxy);
		if (t0 > t1)
		{
			return 0;
		}
	}

	if (rg_absf(ray->dir.z) < RG_EPSILON)
	{
		if (ray->origin.z < aabb->min.z || ray->origin.z > aabb->max.z)
		{
			return 0;
		}
	}
	else
	{
		f32 inv = 1.0f / ray->dir.z;
		f32 tminz = (aabb->min.z - ray->origin.z) * inv;
		f32 tmaxz = (aabb->max.z - ray->origin.z) * inv;
		if (tminz > tmaxz)
		{
			f32 tmp = tminz;
			tminz = tmaxz;
			tmaxz = tmp;
		}
		t0 = rg_maxf(t0, tminz);
		t1 = rg_minf(t1, tmaxz);
		if (t0 > t1)
		{
			return 0;
		}
	}

	if (tmin) *tmin = t0;
	if (tmax) *tmax = t1;

	return t1 >= 0.0f;
}

RGINLINE int rg_aabb_plane(const rg_aabb* aabb, const rg_plane* plane)
{
#if RG_MATH_MAX_PERF
	f32 minx = aabb->min.x;
	f32 miny = aabb->min.y;
	f32 minz = aabb->min.z;
	f32 maxx = aabb->max.x;
	f32 maxy = aabb->max.y;
	f32 maxz = aabb->max.z;
	f32 nx = plane->normal.x;
	f32 ny = plane->normal.y;
	f32 nz = plane->normal.z;
	f32 cx = (minx + maxx) * 0.5f;
	f32 cy = (miny + maxy) * 0.5f;
	f32 cz = (minz + maxz) * 0.5f;
	f32 ex = (maxx - minx) * 0.5f;
	f32 ey = (maxy - miny) * 0.5f;
	f32 ez = (maxz - minz) * 0.5f;
	f32 dist = nx * cx + ny * cy + nz * cz + plane->d;
	f32 radius = rg_absf(nx) * ex + rg_absf(ny) * ey + rg_absf(nz) * ez;
	if (dist + radius < 0.0f)
	{
		return RG_PLANE_OUTSIDE;
	}
	if (dist - radius >= 0.0f)
	{
		return RG_PLANE_INSIDE;
	}
	return RG_PLANE_INTERSECT;
#else
	f32 nx = plane->normal.x;
	f32 ny = plane->normal.y;
	f32 nz = plane->normal.z;

	f32 px = (nx >= 0.0f) ? aabb->max.x : aabb->min.x;
	f32 py = (ny >= 0.0f) ? aabb->max.y : aabb->min.y;
	f32 pz = (nz >= 0.0f) ? aabb->max.z : aabb->min.z;
	f32 dist_p = nx * px + ny * py + nz * pz + plane->d;

	if (dist_p < 0.0f)
	{
		return RG_PLANE_OUTSIDE;
	}

	f32 nxv = (nx >= 0.0f) ? aabb->min.x : aabb->max.x;
	f32 nyv = (ny >= 0.0f) ? aabb->min.y : aabb->max.y;
	f32 nzv = (nz >= 0.0f) ? aabb->min.z : aabb->max.z;
	f32 dist_n = nx * nxv + ny * nyv + nz * nzv + plane->d;

	if (dist_n >= 0.0f)
	{
		return RG_PLANE_INSIDE;
	}

	return RG_PLANE_INTERSECT;
#endif
}

RGINLINE int rg_aabb_sphere(const rg_aabb* aabb, const rg_sphere* sphere)
{
	f32 cx = sphere->center.x;
	f32 cy = sphere->center.y;
	f32 cz = sphere->center.z;
	f32 dx = rg_maxf(rg_maxf(aabb->min.x - cx, 0.0f), cx - aabb->max.x);
	f32 dy = rg_maxf(rg_maxf(aabb->min.y - cy, 0.0f), cy - aabb->max.y);
	f32 dz = rg_maxf(rg_maxf(aabb->min.z - cz, 0.0f), cz - aabb->max.z);
	f32 dist2 = dx * dx + dy * dy + dz * dz;
	return dist2 <= sphere->radius * sphere->radius;
}

RGINLINE void rg_barycentric(const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, const rg_vec3* p, rg_vec3* out)
{
	f32 v0x = b->x - a->x;
	f32 v0y = b->y - a->y;
	f32 v0z = b->z - a->z;
	f32 v1x = c->x - a->x;
	f32 v1y = c->y - a->y;
	f32 v1z = c->z - a->z;
	f32 v2x = p->x - a->x;
	f32 v2y = p->y - a->y;
	f32 v2z = p->z - a->z;

	f32 d00 = v0x * v0x + v0y * v0y + v0z * v0z;
	f32 d01 = v0x * v1x + v0y * v1y + v0z * v1z;
	f32 d11 = v1x * v1x + v1y * v1y + v1z * v1z;
	f32 d20 = v2x * v0x + v2y * v0y + v2z * v0z;
	f32 d21 = v2x * v1x + v2y * v1y + v2z * v1z;

	f32 denom = d00 * d11 - d01 * d01;
	if (denom == 0.0f)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return;
	}

#if RG_MATH_MAX_PERF
	f32 inv_denom = 1.0f / denom;
	f32 v = (d11 * d20 - d01 * d21) * inv_denom;
	f32 w = (d00 * d21 - d01 * d20) * inv_denom;
#else
	f32 v = (d11 * d20 - d01 * d21) / denom;
	f32 w = (d00 * d21 - d01 * d20) / denom;
#endif
	f32 u = 1.0f - v - w;
	out->x = u;
	out->y = v;
	out->z = w;
}

RGINLINE int rg_point_in_triangle(const rg_vec3* a, const rg_vec3* b, const rg_vec3* c, const rg_vec3* p)
{
	f32 v0x = b->x - a->x;
	f32 v0y = b->y - a->y;
	f32 v0z = b->z - a->z;
	f32 v1x = c->x - a->x;
	f32 v1y = c->y - a->y;
	f32 v1z = c->z - a->z;
	f32 v2x = p->x - a->x;
	f32 v2y = p->y - a->y;
	f32 v2z = p->z - a->z;

	f32 d00 = v0x * v0x + v0y * v0y + v0z * v0z;
	f32 d01 = v0x * v1x + v0y * v1y + v0z * v1z;
	f32 d11 = v1x * v1x + v1y * v1y + v1z * v1z;
	f32 d20 = v2x * v0x + v2y * v0y + v2z * v0z;
	f32 d21 = v2x * v1x + v2y * v1y + v2z * v1z;

	f32 denom = d00 * d11 - d01 * d01;
	if (denom == 0.0f)
	{
		return 1;
	}

#if RG_MATH_MAX_PERF
	f32 inv_denom = 1.0f / denom;
	f32 v = (d11 * d20 - d01 * d21) * inv_denom;
	f32 w = (d00 * d21 - d01 * d20) * inv_denom;
#else
	f32 v = (d11 * d20 - d01 * d21) / denom;
	f32 w = (d00 * d21 - d01 * d20) / denom;
#endif
	return (v >= -RG_EPSILON) && (w >= -RG_EPSILON) && ((v + w) <= 1.0f + RG_EPSILON);
}

#endif // RG_MATH_GEOM_H
