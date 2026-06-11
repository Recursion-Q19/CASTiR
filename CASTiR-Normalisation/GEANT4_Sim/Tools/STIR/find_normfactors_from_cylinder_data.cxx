/*
Copyright 2017 ETH Zurich, Institute of Particle Physics and Astrophysics

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*!

  \file
  \ingroup utilities

  \brief Find normalisation factors given projection data of a cylinder (direct method)

  \author Parisa Khateri

*/

#include "stir/utilities.h"
#include <iostream>
#include <fstream>
#include <string>
#include "stir/shared_ptr.h"
#include "stir/ProjData.h"
#include "stir/CartesianCoordinate3D.h"
#include "stir/LORCoordinates.h"
#include "stir/Bin.h"
#include "stir/ProjDataInterfile.h"
#include "stir/IO/interfile.h"
#include <limits>

USING_NAMESPACE_STIR

int
main(int argc, char** argv)
{
  if (argc != 5)
    {
      std::cerr << "Usage: " << argv[0] << " output_file_name_prefix cylider_measured_data cylinder_radius_inner(mm) cylinder_radius_outer(mm)\n"
                << "only cylinder data are supported. The radius should be the radius of the measured cylinder data.\n"
                << "warning: mind the input order\n";
      return EXIT_FAILURE;
    }

  shared_ptr<ProjData> cylinder_projdata_ptr = ProjData::read_from_file(argv[2]); //Maybe later I should change the name to something for annular phantom
  const std::string output_file_name = argv[1];
  const float R_in = atof(argv[3]); // cylinder radius_inner (mm)
  const float R_out = atof(argv[4]); // cylinder radius_outer (mm)

  if (R_out == 0)
    {
      std::cerr << " Radius must be a float value\n"
                << "Usage: " << argv[0] << " output_file_name_prefix cylider_measured_data cylinder_radius_inner cylinder_radius_outer\n"
                << "warning: mind the input order\n";
      return EXIT_FAILURE;
    }

  // output file
  shared_ptr<ProjDataInfo> cylinder_pdi_ptr(cylinder_projdata_ptr->get_proj_data_info_sptr()->clone());

  ProjDataInterfile output_projdata(cylinder_projdata_ptr->get_exam_info_sptr(), cylinder_pdi_ptr, output_file_name);
  write_basic_interfile_PDFS_header(output_file_name, output_projdata);

  CartesianCoordinate3D<float> c1_out, c2_out, c1_in, c2_in;
  LORInAxialAndNoArcCorrSinogramCoordinates<float> lor;

  // first find the average number of counts per LOR
  float total_count = 0;
  float min_count = std::numeric_limits<double>::max(); // minimum number of counts per LOR
  float average_count = 0;                              // average number of counts per LOR in the active region
  int num_active_LORs = 0;                              // number of LORs which pass through the cylinder
  for (int seg = cylinder_projdata_ptr->get_min_segment_num(); seg <= cylinder_projdata_ptr->get_max_segment_num(); ++seg)
    for (int view = cylinder_projdata_ptr->get_min_view_num(); view <= cylinder_projdata_ptr->get_max_view_num(); ++view)
      {
        Viewgram<float> cylinder_viewgram = cylinder_projdata_ptr->get_viewgram(view, seg);
        for (int ax = cylinder_projdata_ptr->get_min_axial_pos_num(seg); ax <= cylinder_projdata_ptr->get_max_axial_pos_num(seg);
             ++ax)
          for (int tang = cylinder_projdata_ptr->get_min_tangential_pos_num();
               tang <= cylinder_projdata_ptr->get_max_tangential_pos_num();
               ++tang)
            { 
              Bin bin(seg, view, ax, tang);
              cylinder_projdata_ptr->get_proj_data_info_sptr()->get_LOR(lor, bin);
              LORAs2Points<float> lor_as2points(lor);
              LORAs2Points<float> intersection_coords;
              if (find_LOR_intersections_with_cylinder(intersection_coords, lor_as2points, R_out) == Succeeded::yes)  //changed by SM: _out was added by seyyed
                {                                            // this only succeeds if LOR is intersecting with the cylinder
                  float N_lor = cylinder_viewgram[ax][tang]; // counts seen by this lor
                  c1_out = intersection_coords.p1(); //changed by SM: _out was added by seyyed
                  c2_out = intersection_coords.p2();
                  float c12_out = sqrt(pow(c1_out.z() - c2_out.z(), 2) // length of intersection of lor with the cylinder // changed by SM: c12 to c12_out
                                   + pow(c1_out.y() - c2_out.y(), 2) + pow(c1_out.x() - c2_out.x(), 2));
                  if (find_LOR_intersections_with_cylinder(intersection_coords, lor_as2points, R_in) == Succeeded::yes) //Added by SM
                    { // all in this brace - Added by SM
                      float N_lor = cylinder_viewgram[ax][tang]; // counts seen by this lor
                      c1_in = intersection_coords.p1();
                      c2_in = intersection_coords.p2();
                      float c12_in = sqrt(pow(c1_in.z() - c2_in.z(), 2) //
                                   + pow(c1_in.y() - c2_in.y(), 2) + pow(c1_in.x() - c2_in.x(), 2));

                      float c12 = c12_out - c12_in;

                      float N_lor_corrected = N_lor / c12; // corrected for the length
                      total_count += N_lor_corrected;
                      num_active_LORs += 1;
                      if (N_lor_corrected < min_count && N_lor_corrected != 0)
                            min_count = N_lor_corrected;
                    } else // Added by SM
                      { // Added by SM
                  // Commented by SM // if (c12 > 0.5) // if LOR intersection is lager than 0.5 mm, check the count per LOR
                  // Commented by SM //  {
                        float N_lor_corrected = N_lor / c12_out; // corrected for the length // changed by SM: c12 to c12_out
                        total_count += N_lor_corrected;
                        num_active_LORs += 1;
                        if (N_lor_corrected < min_count && N_lor_corrected != 0)
                              min_count = N_lor_corrected;
                      } // Added by SM
                  // Commented by SM //  }
                }
            }
      }

  average_count = total_count / num_active_LORs;
  std::cout << "num_lor, tot_count_per_length_unit, average_count_per_length_unit, non_zero_min_per_length_unit = "
            << num_active_LORs << ", " << total_count << ", " << average_count << ", " << min_count << "\n";

  // find the norm factor per LOR
  for (int seg = cylinder_projdata_ptr->get_min_segment_num(); seg <= cylinder_projdata_ptr->get_max_segment_num(); ++seg)
    for (int view = cylinder_projdata_ptr->get_min_view_num(); view <= cylinder_projdata_ptr->get_max_view_num(); ++view)
      {
        Viewgram<float> cylinder_viewgram = cylinder_projdata_ptr->get_viewgram(view, seg);
        Viewgram<float> out_viewgram = cylinder_projdata_ptr->get_empty_viewgram(view, seg);
        for (int ax = cylinder_projdata_ptr->get_min_axial_pos_num(seg); ax <= cylinder_projdata_ptr->get_max_axial_pos_num(seg);
             ++ax)
          for (int tang = cylinder_projdata_ptr->get_min_tangential_pos_num();
               tang <= cylinder_projdata_ptr->get_max_tangential_pos_num();
               ++tang)
            {
              Bin bin(seg, view, ax, tang);
              cylinder_projdata_ptr->get_proj_data_info_sptr()->get_LOR(lor, bin);
              LORAs2Points<float> lor_as2points(lor);
              LORAs2Points<float> intersection_coords;
              float NF_lor;
              // Commented// if (find_LOR_intersections_with_cylinder(intersection_coords, lor_as2points, R) == Succeeded::yes)
              if (find_LOR_intersections_with_cylinder(intersection_coords, lor_as2points, R_out) == Succeeded::yes) // Added by SM
                { // this only succeeds if LOR is intersecting with the cylinder

                  /*
                    for each lor
                      find_LOR_intersections_with_cylinder => c1 & c2
                      c12 = |c1-c2| = sqrt(dx^2+dy^2+dz^2)
                      N_lor/c12 should be the same for all therefore:
                      NF_lor= <N> / (N_lor/c12)
                  */

                  /* Commented by SM
                  float N_lor = cylinder_viewgram[ax][tang]; // counts seen by this lor
                  c1 = intersection_coords.p1();
                  c2 = intersection_coords.p2();
                  float c12 = sqrt(pow(c1.z() - c2.z(), 2) // length of intersection of lor with the cylinder
                                   + pow(c1.y() - c2.y(), 2) + pow(c1.x() - c2.x(), 2));
                  */
                  float N_lor = cylinder_viewgram[ax][tang]; // counts seen by this lor // Added by SM
                  c1_out = intersection_coords.p1(); // Added by SM
                  c2_out = intersection_coords.p2(); // Added by SM
                  float c12_out = sqrt(pow(c1_out.z() - c2_out.z(), 2) // length of intersection of lor with the cylinder // added by SM
                                   + pow(c1_out.y() - c2_out.y(), 2) + pow(c1_out.x() - c2_out.x(), 2)); // Added by SM
                   if (find_LOR_intersections_with_cylinder(intersection_coords, lor_as2points, R_in) == Succeeded::yes) //Added by SM
                    { // all in this brace - Added by SM
                      float N_lor = cylinder_viewgram[ax][tang]; // counts seen by this lor
                      c1_in = intersection_coords.p1();
                      c2_in = intersection_coords.p2();
                      float c12_in = sqrt(pow(c1_in.z() - c2_in.z(), 2) //
                                   + pow(c1_in.y() - c2_in.y(), 2) + pow(c1_in.x() - c2_in.x(), 2));

                      float c12 = c12_out - c12_in;

                      if (N_lor < 1) // if inside the cylinder but the value is too small
                        {
                          NF_lor = average_count * c12 / min_count;
                        }
                      else
                        NF_lor = average_count * c12 / N_lor;
                    }
                  else // Added by SM
                    { // all in this brace - Added by SM
                      if (N_lor < 1) // if inside the cylinder but the value is too small // SM: We might not need this? Am I correct?
                        {
                          NF_lor = average_count * c12_out / min_count;
                        }
                      else
                        NF_lor = average_count * c12_out / N_lor;
                    }
                    

                  /* 
                   Commented by SM 
                  if (N_lor < 1) // if inside the cylinder but the value is too small
                    {
                      NF_lor = average_count * c12 / min_count;
                    }
                  else
                    NF_lor = average_count * c12 / N_lor;
                  */

                }
              else // if out of the cylinder set it to a small value instead of zero, otherwise normalisation gives strange recon
                   // image.
                {
                  NF_lor = 0.0001;
                }
              out_viewgram[ax][tang] = NF_lor;
            }
        output_projdata.set_viewgram(out_viewgram);
      }
}
