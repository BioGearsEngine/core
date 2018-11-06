#ifndef BIOGEARS_COMMON_THREADWORKCHANNEL_H
#define BIOGEARS_COMMON_THREADWORKCHANNEL_H

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
//!  Concurrent Queue using std::queue std::mutex.
//!

#include <memory>
#include <functional>

#include <biogears/framework/mcmp/mcmp_sink_source.h>
#include <biogears/container/concurrent_queue.h>

namespace biogears {
	template<typename Signiture>
	class ThreadWorkChannel : public mcmp::Container<std::function<Signiture>>
		, public std::enable_shared_from_this<ThreadWorkChannel<Signiture>> {
	public:
		using SourcePtr = std::shared_ptr<biogears::mcmp::Source<std::function<Signiture>>>;
		using SinkPtr = std::shared_ptr<biogears::mcmp::Sink<std::function<Signiture>>>;

		static auto make_shared()->std::shared_ptr<ThreadWorkChannel>;
		bool insert(std::function<Signiture>&) final;
		bool insert(std::function<Signiture>&&) final;
		void consume(std::function<Signiture>&) final;
		auto consume()->std::function<Signiture> final;

		size_t unsafe_size() const final;


		void abort() final;
		void shutdown() final;

		SourcePtr as_source() final;
		SinkPtr   as_sink() final;
	protected:
		ThreadWorkChannel();
	private:
		biogears::ConcurrentQueue<std::function<Signiture>> _queue;
	};
}

#endif //BIOGEARS_COMMON_THREADWORKCHANNEL_H