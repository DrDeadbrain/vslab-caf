#pragma once

#include <vector>

#include <caf/all.hpp>

CAF_PUSH_WARNINGS
#include <boost/multiprecision/cpp_int.hpp>
CAF_POP_WARNINGS

CAF_BEGIN_TYPE_ID_BLOCK(vslab, first_custom_type_id)

// -- types ------------------------------------------------------------------

CAF_ADD_TYPE_ID(vslab, (boost::multiprecision::int512_t))
CAF_ADD_TYPE_ID(vslab, (std::vector<boost::multiprecision::int512_t>))

// -- atoms ------------------------------------------------------------------

CAF_ADD_ATOM(vslab, calc_atom)

//CLIENT TO CLIENT OVER GRP
CAF_ADD_ATOM(vslab, init_num_atom);
CAF_ADD_ATOM(vslab, done_msg_atom);

//CLIENT TO WORKER OVER GRP
CAF_ADD_ATOM(vslab, new_num_atom);
CAF_ADD_ATOM(vslab, client_num_atom);
CAF_ADD_ATOM(vslab, block_false_atom);

//WORKER TO CLIENT OVER GRP
CAF_ADD_ATOM(vslab, result_atom);
CAF_ADD_ATOM(vslab, give_number_atom);

CAF_END_TYPE_ID_BLOCK(vslab)
