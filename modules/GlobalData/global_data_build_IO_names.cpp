#include "global_data.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static std::vector<std::string> create_rnd_vector_file_names (
                                    const int config, const int nb_of_eigen_vec,
                                    const std::vector<quark> quarks) {

  std::vector<std::string> filename_list; // the return vector of file names
  // running over all quarks
  for(const auto& q: quarks){

    char temp1[200];
    char temp2[200];

    for(int rnd_vec_i = 0; rnd_vec_i < q.number_of_rnd_vec; ++rnd_vec_i){
      // building paths and filenames for rnd vecs
      sprintf(temp1, "cnfg%d/rnd_vec_%01d/", config, rnd_vec_i);
      sprintf(temp2, "randomvector.rndvecnb%02d.%s.nbev%04d.%04d", rnd_vec_i, 
                                       q.type.c_str(), nb_of_eigen_vec, config);
      filename_list.push_back(q.path + "/" + temp1 + temp2);
    }
  }
  return filename_list;
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static std::vector<std::string> create_perambulator_file_names (
                                              const int config, const int Lt,
                                              const int Lx, const int Ly, 
                                              const int Lz, 
                                              const std::vector<quark> quarks) {

  std::vector<std::string> filename_list; // the return vector of file names
  // running over all quarks
  for(const auto& q: quarks){

    char temp1[200];
    char temp2[200];

    for(int rnd_vec_i = 0; rnd_vec_i < q.number_of_rnd_vec; ++rnd_vec_i){
      // data path for qbig contractions
      sprintf(temp1, "cnfg%d/rnd_vec_%01d/", config, rnd_vec_i);

      sprintf(temp2,
          "perambulator.rndvecnb%02d.%s.TsoB%04d.VsoI%04d.DsoF%1d.TsiF%04d."
          "SsiF%d.DsiF4.CsiF3.smeared1.%05d", 
          rnd_vec_i, q.type.c_str(), Lt / q.number_of_dilution_T, 
          q.number_of_dilution_E, q.number_of_dilution_D, Lt, Lx*Ly*Lz, config);
      filename_list.push_back(q.path + "/" + temp1 + temp2);
    }
  }
  return filename_list;
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static std::string create_eigenvector_file_name (
                                         const size_t config,
                                         const std::string& path_eigenvectors,
                                         const std::string& name_eigenvectors) {
  char name[200];
  std::string filename = path_eigenvectors + "/" + name_eigenvectors;
  sprintf(name, "%s.%04d.", filename.c_str(), (int) config);
  return name;

}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void GlobalData::build_IO_names(const size_t config){

  rnd_vec_construct.filename_list = create_rnd_vector_file_names(
                                           config, number_of_eigen_vec, quarks);
  peram_construct.filename_list = create_perambulator_file_names(
                                                config, Lt, Lx, Ly, Lz, quarks);
  filename_eigenvectors = create_eigenvector_file_name(config, 
                                                       path_eigenvectors, 
                                                       name_eigenvectors);
}
