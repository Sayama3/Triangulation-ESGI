[![build](https://github.com/Sayama3/Triangulation-ESGI/actions/workflows/cmake.yml/badge.svg)](https://github.com/Sayama3/Triangulation-ESGI/actions/workflows/cmake.yml)

# Triangulation Project

This repository is a school project to work on 2D and 3D triangulation.
We've made it using :

- [Raylib](https://www.raylib.com/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [rlImGui](https://github.com/raylib-extras/rlImGui)
- [glm](https://github.com/g-truc/glm)

The goals are :
- [ ] Be able to make a 2D convex envelope of a point cloud.
- [ ] Be able to triangulate the 2D point cloud.
- [ ] Use Delaunay method to triangulate the point cloud
- [ ] Create a 2D Voronoi Diagram
- [ ] Implement a 3D Convex Envelop
- [ ] Triangulate the 3D Convex Envelop using Delaunay method
- [ ] Try to implement a 3D Voronoi Diagram

To do so, we've separated the Math library and the application to maybe reuse it later, 
but more importantly, be able to add Unit Tests and verify if our algorithm works.