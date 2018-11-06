#ifndef BIOGEARS_FRAMEWORK_SCMP_CONTAINER_H
#define BIOGEARS_FRAMEWORK_SCMP_CONTAINER_H

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
//! \date   Sept 15 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license   This project is released under the Apache 2.0 license.
//! \file 
//!
//!  \brief Template interface for single-consumer
//!   multi producer channel.  With associated Source
//!  interface to be handed to producers.

#include <cstddef>
#include <memory>
namespace biogears {
  namespace scmp {
    template<class C>
    class Channel;

    //!  Source interface for a Channel
    //!  stores a shared_ptr to the
    //!  container of the channel that constructed it.
    template<class C>
    class Source {
      friend class Channel<C>;
      using CPtr = std::shared_ptr<C>;
    public:
      Source() = delete;
      Source(const Source&) = default;
      Source( Source&&) = default;

      ~Source();

      bool insert( typename C::value_type&  );
      bool insert( typename C::value_type&& );

      bool active();   //< returns if the underlying channel is still active
    protected:
      Source( CPtr );
      static std::unique_ptr<Source<C>> make_unique(std::shared_ptr<C>);
    private:
      CPtr _container;
    };

    //!  Single consumer multi producer
    //!  Channel for concurrent programming
    template<class C>
    class Channel {
      using CPtr = std::shared_ptr<C>;
    public:
      template<typename... Args>
      Channel(Args&& ...);

      Channel();  //<! Stands up container internally
      Channel( std::unique_ptr<C> ); //<! Moves Container internally
      ~Channel();

      void consume ( typename C::value_type&  ) ;
      auto consume() -> typename C::value_type  ;

      void peek ( const typename C::value_type&  ) const;
      auto peek() const -> const typename C::value_type&;

      size_t size() const;

      void abort();    //< Signals all source to abort any pending insets
      void shutdown(); //< Sets the queue to inoperable and calls abort
      bool active();   //< returns if the underlying channel is still active
      Source<C> as_source();
    private:
      CPtr _container;
    };

  }
}

#endif //BIOGEARS_FRAMEWORK_SCMP_CONTAINER_H
