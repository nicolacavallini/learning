CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

set(DEAL_II_DIR /home/nicola/local/usr/dealii/)

FIND_PACKAGE(deal.II 8.5.0 REQUIRED
  HINTS ${DEAL_II_DIR} ../ ../../ $ENV{DEAL_II_DIR}
  )
DEAL_II_INITIALIZE_CACHED_VARIABLES()

PROJECT(evaluate_glcm)

ADD_EXECUTABLE(evaluate_glcm evaluate_glcm.cc)
DEAL_II_SETUP_TARGET(evaluate_glcm)
