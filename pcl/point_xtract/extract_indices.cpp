#include <iostream>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>

bool segment_planar_component(const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered,
                              pcl::PointIndices::Ptr inliers,
                              pcl::ModelCoefficients::Ptr coefficients){

  // Create the segmentation object
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  // Optional
  seg.setOptimizeCoefficients (true);
  // Mandatory
  seg.setModelType (pcl::SACMODEL_PLANE);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (1000);
  seg.setDistanceThreshold (0.01);

      std::cerr << "Point remaining from the original cloud : " << cloud_filtered->width * cloud_filtered->height << " data points." << std::endl;
    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud (cloud_filtered);
    seg.segment (*inliers, *coefficients);
    if (inliers->indices.size () == 0)
      return true;
    else 
      return false;
}

void remove_inliers_from_cloud(const pcl::PointIndices::Ptr &inliers,
                               pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud_filtered){

    // Create the filtering object
    pcl::ExtractIndices<pcl::PointXYZ> extract;

    // Extract the inliers
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_p (new pcl::PointCloud<pcl::PointXYZ>);
    extract.setInputCloud (cloud_filtered);
    extract.setIndices (inliers);
    extract.setNegative (false);
    extract.filter (*cloud_p);
    std::cerr << "PointCloud representing the planar component: " 
              << cloud_p->width * cloud_p->height 
              << " data points." << std::endl;

    //std::stringstream ss;
    //ss << "table_scene_lms400_plane_" << i << ".pcd";
    //writer.write<pcl::PointXYZ> (ss.str (), *cloud_p, false);

    // Create the filtering object
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
    extract.setNegative (true);
    extract.filter (*cloud_f);
    cloud_filtered.swap (cloud_f);

}

int
main (int argc, char** argv)
{

  // Fill in the cloud data
  pcl::PCLPointCloud2::Ptr cloud_blob (new pcl::PCLPointCloud2);
  pcl::PCDReader reader;
  reader.read ("table_scene_lms400.pcd", *cloud_blob);

  std::cerr << "PointCloud before filtering: " << cloud_blob->width * cloud_blob->height << " data points." << std::endl;

  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  pcl::PCLPointCloud2::Ptr cloud_filtered_blob (new pcl::PCLPointCloud2);
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloud_blob);
  sor.setLeafSize (0.01f, 0.01f, 0.01f);
  sor.filter (*cloud_filtered_blob);

  // Convert to the templated PointCloud
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromPCLPointCloud2 (*cloud_filtered_blob, *cloud_filtered);

  std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height << " data points." << std::endl;

  // Write the downsampled version to disk
  pcl::PCDWriter writer;
  writer.write<pcl::PointXYZ> ("table_scene_lms400_downsampled.pcd", *cloud_filtered, false);



  int i = 0, nr_points = (int) cloud_filtered->points.size ();
  // While 30% of the original cloud is still there



  while (cloud_filtered->points.size () > 0.3 * nr_points)
  {    
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices ());

    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());


    if (segment_planar_component(cloud_filtered,inliers,coefficients)) 
      std::cerr << "Could not estimate a planar model for the given dataset." << std::endl;

    
    remove_inliers_from_cloud(inliers,cloud_filtered);


    i++;
  }

  return (0);
}