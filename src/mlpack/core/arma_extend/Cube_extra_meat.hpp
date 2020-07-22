// Copyright (C) 2008-2016 National ICT Australia (NICTA)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// -------------------------------------------------------------------
//
// Written by Conrad Sanderson - http://conradsanderson.id.au
// Written by Ryan Curtin

// Add a serialization function
template<typename Archive, typename eT>
void serialize(Archive& ar, arma::Cube<eT>& cube)
{
  const uword old_n_elem = cube.n_elem;

  // This is accurate from Armadillo 3.6.0 onwards.
  ar(cereal::make_nvp("n_rows", access::rw(cube.n_rows)));
  ar(cereal::make_nvp("n_cols", access::rw(cube.n_cols)));
  ar(cereal::make_nvp("n_elem_slice", access::rw(cube.n_elem_slice)));
  ar(cereal::make_nvp("n_slices", access::rw(cube.n_slices)));
  ar(cereal::make_nvp("n_elem", access::rw(cube.n_elem)));

  // mem_state will always be 0 on load, so we don't need to save it.
  if (Archive::is_loading::value)
  {
    // Clean any mat pointers.
    delete_mat();

    // Don't free if local memory is being used.
    if (cube.mem_state == 0 && cube.mem != NULL && old_n_elem > arma_config::mat_prealloc)
    {
      memory::release(access::rw(cube.mem));
    }

    access::rw(cube.mem_state) = 0;

    // We also need to allocate the memory we're using.
    init_cold();
  }

  ar & cereal::make_array(access::rwp(cube.mem), cube.n_elem);
}
