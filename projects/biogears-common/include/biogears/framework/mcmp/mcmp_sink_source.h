#ifndef BIOGEARS_FRAMEWORK_CHANNEL_H
#define BIOGEARS_FRAMEWORK_CHANNEL_H

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

//! \author Steven A White
//! \date   Sept 7 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//! \brief Abstract interfaces to define a multi-consumer
//!   multi-producer thread safe container witch limited
//!   Sink and Source interfaces to be handed to consumer
//!   and producers respectively.  Implementation is left
//!   as an exercise to the reader
//!
//!   multi-producer multi-consumer thread safe container interface

#include <cstddef>
#include <memory>
namespace biogears {
  namespace mcmp {
    template<typename T>
    class Source {
    public:
      virtual ~Source() = default;

      virtual bool insert( T& ) = 0;
      virtual bool insert( T&& )= 0;
    protected:
      Source() = default;
    };


    template<typename T>
    class Sink {
    public:
      virtual ~Sink() = default;

      virtual void consume(T&) = 0;
      virtual auto consume() -> T = 0;

      virtual size_t unsafe_size() const = 0;
    protected:
      Sink() = default;
    };

    template<typename T>
    class Container: virtual public Source<T>, virtual public Sink<T> {
    public:
			using SourcePtr = std::shared_ptr<Source<T>>;
			using SinkPtr   = std::shared_ptr<Sink<T>>;
      virtual ~Container() {};

      virtual bool insert( T& ) override = 0;
      virtual bool insert( T&& )override= 0;
      virtual void consume(T&) override= 0;
      virtual auto consume() -> T override = 0;

      virtual size_t unsafe_size() const override = 0;

      virtual void abort() = 0;
      virtual void shutdown() = 0;

      virtual SourcePtr as_source() = 0;
      virtual SinkPtr   as_sink()   = 0;
    protected:
      Container() = default;
    };
  }
}

#endif //BIOGEARS_FRAMEWORK_CHANNEL_H
