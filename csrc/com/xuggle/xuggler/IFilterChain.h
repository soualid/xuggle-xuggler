/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IFilterChain.h
 * Author: ayache
 *
 * Created on March 9, 2020, 11:01 PM
 */

#ifndef IFILTERCHAIN_H
#define IFILTERCHAIN_H

#include <com/xuggle/ferry/RefCounted.h>
#include <com/xuggle/xuggler/Xuggler.h>
#include <com/xuggle/xuggler/IMediaFilter.h>
#include <com/xuggle/xuggler/IAudioSamples.h>
#include <com/xuggle/xuggler/IBufferSource.h>
#include <com/xuggle/xuggler/IBufferSink.h>

namespace com {
    namespace xuggle {
        namespace xuggler {

            class VS_API_XUGGLER IFilterChain : public com::xuggle::ferry::RefCounted {
            public:
                
                /**
                 * @return a new filter chain
                 */
                static IFilterChain* make();
                
                /**
                 * Create a new filter from name
                 * 
                 * @param name the name of the filter
                 * @return a new filter or NULL on error
                 * 
                 */
                virtual IMediaFilter* createFilter(const char* name) = 0;
                
                /**
                 * 
                 * @return a new source for this chain
                 * 
                 */
                virtual IBufferSource* createSource(IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base) = 0;
                
                /**
                 *
                 * @return a new sink for this chain
                 * 
                 */
                virtual IBufferSink* createSink() = 0;
                
                /**
                 * Configures the chain
                 * 
                 * @return 0 on success
                 */
                virtual int configure() = 0;
                
                IFilterChain();
                virtual ~IFilterChain();
            private:

            };

        }
    }
}

#endif /* IFILTERCHAIN_H */

