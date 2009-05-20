/*
 * Copyright (c) 2008, 2009 by Xuggle Incorporated.  All rights reserved.
 * 
 * This file is part of Xuggler.
 * 
 * You can redistribute Xuggler and/or modify it under the terms of the GNU
 * Affero General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any
 * later version.
 * 
 * Xuggler is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with Xuggler.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <com/xuggle/ferry/Logger.h>
#include <com/xuggle/ferry/RefPointer.h>
#include <com/xuggle/xuggler/Global.h>
#include <com/xuggle/xuggler/Codec.h>
#include <com/xuggle/xuggler/ContainerFormat.h>

VS_LOG_SETUP(VS_CPP_PACKAGE);

namespace com { namespace xuggle { namespace xuggler
{
  using namespace com::xuggle::ferry;

  Codec :: Codec()
  {
    VS_LOG_TRACE("Starting");
    mCodec = 0;
  }

  Codec :: ~Codec()
  {
    // no memory freeing required.
    mCodec = 0;
  }

  const char *
  Codec :: getName() {
    const char * retval = 0;
    if (mCodec)
      retval = mCodec->name;
    return retval;
  }

  int
  Codec :: getIDAsInt()
  {
    int retval = CODEC_ID_NONE;
    if (mCodec)
      retval = mCodec->id;
    return retval;
  }

  ICodec::Type
  Codec :: getType()
  {
    Type retval = CODEC_TYPE_UNKNOWN;
    if (mCodec)
      retval = (Type) mCodec->type;
    return retval;
  }

  Codec *
  Codec :: make(AVCodec *aCodec)
  {
    Codec *retval = 0;
    if (aCodec)
    {
      retval = Codec::make();
      if (retval)
        retval->mCodec = aCodec;
    }
    return retval;
  }

  Codec *
  Codec :: findEncodingCodec(const ICodec::ID id)
  {
    return Codec::findEncodingCodecByIntID((const int)id);
  }
  Codec *
  Codec :: findEncodingCodecByIntID(const int id)
  {
    Codec *retval = 0;
    AVCodec *codec = 0;
    enum CodecID ffmpeg_id = (enum CodecID) id;
    Global::lock();
    codec = avcodec_find_encoder(ffmpeg_id);
    Global::unlock();
    if (codec)
      retval = Codec::make(codec);

    return retval;
  }

  Codec *
  Codec :: findEncodingCodecByName(const char* name)
  {
    Codec *retval = 0;
    AVCodec *codec = 0;
    if (name && *name)
    {
      Global::lock();
      codec = avcodec_find_encoder_by_name(name);
      Global::unlock();
      if (codec)
        retval = Codec::make(codec);
    }
    return retval;
  }

  Codec *
  Codec :: findDecodingCodec(const ICodec::ID id)
  {
    return Codec::findDecodingCodecByIntID((const int)id);
  }

  Codec *
  Codec :: findDecodingCodecByIntID(const int id)
  {
    Codec *retval = 0;
    AVCodec *codec = 0;

    Global::lock();
    codec = avcodec_find_decoder((enum CodecID) id);
    Global::unlock();
    
    if (codec)
      retval = Codec::make(codec);
    return retval;
  }

  Codec *
  Codec :: findDecodingCodecByName(const char* name)
  {
    Codec *retval = 0;
    AVCodec *codec = 0;
    if (name && *name)
    {
      Global::lock();
      codec = avcodec_find_decoder_by_name(name);
      Global::unlock();
      if (codec)
        retval = Codec::make(codec);
    }
    return retval;
  }
  bool
  Codec :: canDecode()
  {
    return mCodec ? mCodec->decode : false;
  }

  bool
  Codec :: canEncode()
  {
    return mCodec ? mCodec->encode : false;
  }

  Codec*
  Codec :: guessEncodingCodec(IContainerFormat* pFmt,
      const char* shortName,
      const char* url,
      const char* mimeType,
      ICodec::Type type)
  {
    Codec* retval = 0;
    RefPointer<ContainerFormat> fmt = 0;
    AVOutputFormat * oFmt = 0;

    // We acquire here because the RefPointer always
    // releases.
    fmt.reset(dynamic_cast<ContainerFormat*>(pFmt), true);

    if (!fmt)
    {
      fmt = ContainerFormat::make();
      if (fmt)
        fmt->setOutputFormat(shortName, url, mimeType);
    }
    if (fmt)
      oFmt = fmt->getOutputFormat();

    // Make sure at least one in put is specified.
    // The av_guess_codec function REQUIRES a
    // non null AVOutputFormat
    // It also examines url with a strcmp(), so let's
    // give it a zero-length string.
    // In reality, it ignores the other params.
    if (!url)
      url = "";

    if (oFmt)
    {
      enum CodecID id = av_guess_codec(oFmt, shortName, url,
          mimeType, (enum CodecType) type);
      retval = Codec::findEncodingCodecByIntID((int)id);
    }
    return retval;
  }

  int32_t
  Codec :: acquire()
  {
    int retval = 0;
    retval = RefCounted::acquire();
    VS_LOG_TRACE("Acquired %p: %d", this, retval);
    return retval;
  }
  int32_t
  Codec :: release()
  {
    int retval = 0;
    retval = RefCounted::release();
    VS_LOG_TRACE("Released %p: %d", this, retval);
    return retval;
  }
  const char *
  Codec :: getLongName()
  {
    return mCodec ? mCodec->long_name : 0;
  }
}}}
