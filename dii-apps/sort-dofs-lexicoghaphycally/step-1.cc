/* ---------------------------------------------------------------------
 *
 * Copyright (C) 1999 - 2022 by the deal.II authors
 *
 * This file is part of the deal.II library.
 *
 * The deal.II library is free software; you can use it, redistribute
 * it, and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * The full text of the license can be found in the file LICENSE.md at
 * the top level directory of deal.II.
 *
 * ---------------------------------------------------------------------
 */

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_out.h>

#include <deal.II/fe/fe_q.h>
#include <deal.II/dofs/dof_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/mapping_q1.h>


#include <iostream>
#include <fstream>
#include <cmath>

using namespace dealii;

  template <int dim>
  bool point_comparison(const Point<dim> &lhs, const Point<dim> &rhs) {

    for (int d = dim-1; d > 0; --d)
    {
      if (lhs[d] < rhs[d])       return true;
      if (rhs[d] < lhs[d])       return false;
    }

    return lhs[0] < rhs[0];
  }

  template <typename T>
  std::vector<std::size_t> sort_points(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  std::stable_sort(idx.begin(), idx.end(),
       [&v](std::size_t i1, std::size_t i2) {return point_comparison(v[i1] , v[i2]);});

  return idx;
  }

template <int dim>
void first_grid()
{
  Triangulation<dim> triangulation;

  GridGenerator::hyper_cube(triangulation, -1, 1);
  triangulation.refine_global(1);

  DoFHandler<dim> dof_handler(triangulation);

  const FE_Q<dim> fe(1);
  dof_handler.distribute_dofs(fe);



  const std::map<types::global_dof_index, Point<dim>> dof_location_map =
      DoFTools::map_dofs_to_support_points(MappingQ<dim>(fe.degree), dof_handler);


  std::vector<Point<dim>> dof_locations;

    for (const auto &dof_location : dof_location_map)
    {
      std::cout << "dof " << dof_location.first
                << " is at " << dof_location.second << std::endl;
      dof_locations.push_back(dof_location.second);
    }
    
    std::vector<std::size_t> sorted_indexes = sort_points(dof_locations);

    std::cout << "Sorted DOF locations:" << std::endl;
    for (const auto &index : sorted_indexes)
    {
      std::cout << "DOF " << index << ": " << dof_locations[index] << std::endl;
    }
    std::cout << "Total DOFs: " << dof_locations.size() << std::endl;
}

int main()
{
  first_grid<2>();
  first_grid<3>();
}
