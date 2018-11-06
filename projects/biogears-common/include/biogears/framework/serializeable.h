#ifndef BIOGEARS_FRAMEWORK_SERIALIZABLE_H
#define BIOGEARS_FRAMEWORK_SERIALIZABLE_H

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! \author Steven A White
//! \date November 15th 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file
//!
//! Interface for serializable objects
//! Instead implement the following in the derived class
//!

#include <iostream>

namespace biogears {

	class serializable {
	public:
    virtual ~serializable() = default;

		virtual void serialize( std::ostream& ) const = 0;
		virtual void serialize_text(std::ostream&) const = 0;
		virtual void deserialize(std::istream&) = 0;
		virtual void deserialize_text(std::istream&) = 0;
	};
}

// Additionally implement the following in the derived type
//
//  std::ostream& operator<<( std::ostream& os, const derived& obj)
//    {
//       obj.serialize( os );
//       return os;
//    }

//  std::istream& operator>>( std::istream& is, derived& obj)
//  {
//       try {
//         obj.deserialize( is );
//       }  catch ( ... ) {
//          is.setstate(std::ios::failbit);
//       }
//       return is;
//  }
//
#endif //BIOGEARS_FRAMEWORK_SERIALIZABLE_H
