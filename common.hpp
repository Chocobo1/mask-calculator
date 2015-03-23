/*
* (C) 2014 Mike Tzou
*
* This file is part of "Mask calculator".
*
* "Mask calculator" is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* "Mask calculator" is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef common_HPP
#define common_HPP

#include <cstdint>


bool diffOneBit( const uint32_t a, const uint32_t b );
bool checkMasked( const uint32_t mask , const uint32_t a , const uint32_t b );

#endif
