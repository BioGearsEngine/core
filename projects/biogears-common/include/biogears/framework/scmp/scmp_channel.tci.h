#ifndef BIOGEARS_FRAMEWORK_SCMP_container_TCI_H
#define BIOGEARS_FRAMEWORK_SCMP_container_TCI_H

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
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//!  \brief Template interface for single-consumer
//!   multi producer channel.  With associated Source
//!  interface to be handed to producers.
#include <cstddef>
#include <memory>

#include <biogears/framework/scmp/scmp_channel.h>
namespace biogears {
  namespace scmp {
    //~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
    //! Source
    //~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*

    //!
    //! Constructs a source using the given container
    //!
    template<class C>
    Source<C>::Source( CPtr c)
      :_container(c)
    {}
    //--------------------------------------------------------------------------
    //!
    //! Creates a unique_ptr<Source<C>> from a shared_ptr<C>
    //! This works around Source<C>( ptr) being a private function
    //!
    template<class C>
    std::unique_ptr<Source<C>>  Source<C>::make_unique(std::shared_ptr<C> ptr)
    {
      return std::unique_ptr< Source<C> >(new Source<C>(ptr));
    }
    //--------------------------------------------------------------------------
    //!
    //! Destroys the source and sets its internal container to nullptr
    //!
    template<class C>
    Source<C>::~Source()
    {
      _container = nullptr;
    }
    //--------------------------------------------------------------------------
    //! \param  C::value_type& [INOUT] : New Element to be copied to the queue
    //!                      if size exceeds capacity val could be swapped with
    //!                      the head based on the underlying container.
    //! \return bool : The result of the underlying containers insert
    //! \brief Calls the underlying containers insert
    //!         and is subject to its overflow behavior
    //!
    template<class C>
    bool Source<C>::insert( typename C::value_type& val )
    {
      return  _container->insert(val);
    }
    //--------------------------------------------------------------------------
    //! \param  C::value_type&& [INOUT] : New Element to be moved in the queue
    //!                      if size exceeds capacity val could be swapped with
    //!                      the head based on the underlying container.
    //! \return bool : The result of the underlying containers insert
    //! \brief Calls the underlying containers move insert
    //!         and is subject to its overflow behavior
    //!
    template<class C>
    bool Source<C>::insert( typename C::value_type&& val)
    {
      return _container->insert( std::move(val) );
    }
    //--------------------------------------------------------------------------
    //!
    //! \return bool : The current state of the underlying queue.
    //!
    template<class C>
    bool Source<C>::active( )
    {  return _container->active(); }

    //~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
    //! Channel
    //~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*

    //--------------------------------------------------------------------------
    //!
    //! Constructs a Channel with the given container
    //! The template parameter should implement Container interface
    template<class C>
    Channel<C>::Channel(std::unique_ptr<C>  container)
      :_container(container.release())
    {  }
    //--------------------------------------------------------------------------
    //!
    //! Constructs a Channel and stands up an internal container
    //!
    template<class C>
    Channel<C>::Channel()
      : _container(std::make_shared<C>())
    {  }
    //--------------------------------------------------------------------------
    //!
    //!  Constructs a Channel and stands up an internal container
    //!  Parameters are forwarded to the container
    template<class C>
    template<typename... Args>
    Channel<C>::Channel(Args&& ...args)
      :_container(std::make_shared<C>(args...))
    {}
    //--------------------------------------------------------------------------
    //!
    //! Shutdown the attached container and destroys the channel.
    //! All future source calls will become noops
    //! If the template parameter properly supports shutdown.
    template<class C>
    Channel<C>::~Channel()
    {
      _container->shutdown();
      _container = nullptr;
    }
    //--------------------------------------------------------------------------
    //!
    //!  \param  C::value_type& [OUT] : Result of the dequeue
    //!  \brief Dequeues an item if it exist and is subject
    //!         the container types empty behavior
    template<class C>
    void Channel<C>::consume ( typename C::value_type&  val)
    {
      _container->consume(val);
    }
    //--------------------------------------------------------------------------
    //!
    //!  \return  C::value_type : Result of the dequeue
    //!  \brief Dequeues an item if it exist and is subject
    //!         the container types empty behavior
    template<class C>
    auto Channel<C>::consume() -> typename C::value_type
    {
      return _container->consume();
    }
    //--------------------------------------------------------------------------
    //!
    //!  \param  T& [OUT] : Front of the queue
    //!  \brief Peeks at the front of the queue. This is safe
    //!         as no other consumer exist
    template<class C>
    void Channel<C>::peek ( const typename C::value_type& val ) const
    {
      val = _container->peek();
    }
    //--------------------------------------------------------------------------
    //!
    //!  \return  C::value_type : Front of the queue
    //!  \brief Peeks at the front of the queue. This is safe
    //!         as no other consumer exist
    template<class C>
    auto Channel<C>::peek() const  -> const typename C::value_type&
    {
      return _container->peek();
    }
    //--------------------------------------------------------------------------
    //!
    //!  \return  size_t : Front of the queue
    //!  \brief Peeks at the front of the queue. This is safe
    //!         as no other consumer exist
    template<class C>
    size_t Channel<C>::size() const
    {
      return  _container->unsafe_size();
    }
    //--------------------------------------------------------------------------
    //!
    //! Calls the underlying containers abort function;
    //!
    template<class C>
    void Channel<C>::abort()
    {
      _container->abort();
    }
    //--------------------------------------------------------------------------
    //!
    //! Calls the underlying containers active;
    //!
    template<class C>
    bool Channel<C>::active( )
    {  return _container->active(); }
    //--------------------------------------------------------------------------
    //!
    //! Calls the underlying containers shutdown;
    //!
    template<class C>
    void Channel<C>::shutdown()
    {
      _container->shutdown();
    }
    //--------------------------------------------------------------------------
    //!
    //! Returns the Channel as a Source to be given to producers
    //!
    template<class C>
    auto Channel<C>::as_source() -> Source<C>
    {
      return Source<C>(_container);
    }
  }
}
#endif //BIOGEARS_FRAMEWORK_SCMP_container_TCI_H