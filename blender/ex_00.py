import bpy

from mathutils import Vector

if __name__ == "__main__":
    print("my script")
    bpy.ops.object.delete()
    bpy.ops.surface.primitive_nurbs_surface_surface_add()

    bpy.context.object.data.splines[0].use_endpoint_u=True
    bpy.context.object.data.splines[0].use_endpoint_v=True

    n_points = len(bpy.context.object.data.splines[0].points)

    for i in range(n_points):
        Vector((-1.5, -1.5, 0.0, 1.0))

    for p in bpy.context.object.data.splines[0].points:
        print(p)
