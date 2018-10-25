/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_PARSER_HPP
#define ALPS_EXPRESSION_PARSER_HPP

#include <map>
#include <string>

namespace alps {
namespace expression {

/// \brief reads an ALPS parameter name from a \c std::istream
/// 
/// valid characters, in addition to those in an XML identifier are \c ', 
/// and additionally any arbitrary sequence of characters (including whitespace) surrounded by \c [ ... \ ] 
/// characters, such as in \c MEASURE[Staggered \c Magnetization^2] .
std::string parse_parameter_name(std::istream& in);

/// \brief checks that the next character read from the stream.
/// \param in the stream to be read
/// \param c the character that should be read
/// \param err the error message to be used if the next character is not \a c.
/// \throw \c std::runtime_error( \a err \c ) if the next character is not \a c
///  reads the next character (slipping white space) and checks if it is the same
///  as the character passed as argument \a c and throws a \c std::runtime_error otherwise.
void check_character(std::istream& in, char test, const std::string& err);

} // end namespace expression
} // end namespace alps

#endif
