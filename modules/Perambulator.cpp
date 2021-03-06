#include "Perambulator.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LapH::Perambulator::read_perambulator(const size_t entity, 
                                           const size_t Lt,
                                           const size_t nb_eigen_vec,
                                           const quark& q,
                                           const std::string& filename) {

  clock_t t = clock();
  FILE *fp = NULL;

  std::cout << "\tReading perambulator from file:\n\t\t" << filename;

  // reading the data into temporary array
  vec perambulator_read(peram[entity].size());
  if((fp = fopen(filename.c_str(), "rb")) == NULL){
    std::cout << "failed to open file to read perambulaots: " 
              << filename << "\n" << std::endl;
    exit(0);
  }
  int check_read = fread(&(perambulator_read[0]), sizeof(cmplx),
                                                      peram[entity].size(), fp);
  fclose(fp);
  // check if all data were read in
  if(check_read != peram[entity].size()){
    std::cout << "\n\nFailed to read perambulator\n" << std::endl;
    exit(0);
  }
  
  // setting peram to zero
  peram[entity].setZero();

  // re-sorting and copy into matrix structure 
  // TODO: At this point it is very easy to included different dilution schemes.
  //       However, due to simplicity this will be postponed!
  const size_t nb_dil_T = q.number_of_dilution_T;
  const size_t nb_dil_E = q.number_of_dilution_E;
  const size_t nb_dil_D = q.number_of_dilution_D;
  size_t col_i, row_i;
  const int nb_inversions = Lt * nb_dil_E * nb_dil_D / nb_dil_T;
  for(size_t t1 = 0; t1 < Lt; ++t1)
    for(size_t ev1 = 0; ev1 < nb_eigen_vec; ++ev1)
      for(size_t dirac1 = 0; dirac1 < 4; ++dirac1)
        for(size_t t2 = 0; t2 < (Lt / nb_dil_T); ++t2)
          for(size_t ev2 = 0; ev2 < nb_dil_E; ++ev2)
            for(size_t dirac2 = 0; dirac2 < nb_dil_D; ++dirac2){
              row_i = 4 * nb_eigen_vec * t1 + 4 * ev1 + dirac1;
              col_i = nb_dil_D * nb_dil_E * t2 + nb_dil_D * ev2 + dirac2;
              peram[entity](4 * nb_eigen_vec * t1 + nb_eigen_vec * dirac1 + ev1, 
                    nb_dil_E * nb_dil_D * t2 + nb_dil_E * dirac2 + ev2) = 
              perambulator_read[row_i * nb_inversions + col_i];
            }

  // writing out how long it took to read the file
  t = clock() - t;
  std::cout << "\n\t\tin: " << std::fixed << std::setprecision(1)
            << ((float) t)/CLOCKS_PER_SEC << " seconds" << std::endl;
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LapH::Perambulator::read_perambulators_from_separate_files(
                                 const size_t Lt, const size_t nb_eigen_vec,
                                 const std::vector<quark>& quark,
                                 const std::vector<std::string>&filename_list) {

  if(filename_list.size() != peram.size())
    std::cout << "Problem when reading perambulators: The number of "
              << "perambulators read is not the same as the expected one!" 
              << std::endl;
  size_t j = 0; // TODO: Not beautiful but practical - Think about change
  for(size_t i = 0; i < quark.size(); i++){
    for(size_t r = 0; r < quark[i].number_of_rnd_vec; r++){
      read_perambulator(j, Lt, nb_eigen_vec, quark[i], filename_list[j]);
      j++;
    }
  }
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
