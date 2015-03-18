//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#include <evolution/core/base/Win32.hpp>
#include <evolution/core/base/AcquirableImpl.hpp>
#include <evolution/core/base/AcquirableLock.hpp>
#include <evolution/core/base/Alloca.hpp>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/base/AutoArrayPtr.hpp>
#include <evolution/core/base/BaseC.h>
#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/Configuration.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/EnvironmentSetup.hpp>
#include <evolution/core/base/ERSPLibC.hpp>
#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/FileLoggingOutput.hpp>
#include <evolution/core/base/IAcquirable.hpp>
#include <evolution/core/base/IConditionVariable.hpp>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/base/ILoggingLayout.hpp>
#include <evolution/core/base/ILoggingOutput.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/base/IObject.hpp>
#include <evolution/core/base/IRunnable.hpp>
#include <evolution/core/base/ISharedLibrary.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/base/LoggingCategory.hpp>
#include <evolution/core/base/LoggingHierarchy.hpp>
#include <evolution/core/base/Logging.hpp>
#include <evolution/core/base/LoggingLayoutImpl.hpp>
#include <evolution/core/base/LoggingOutputImpl.hpp>
#include <evolution/core/base/LoggingTypes.hpp>
#include <evolution/core/base/NullConfigSource.hpp>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/base/Paths.hpp>
#include <evolution/core/base/Platform.hpp>
#include <evolution/core/base/RefPtr.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/SimpleProfiler.hpp>
#include <evolution/core/base/StringUtils.hpp>
#include <evolution/core/base/TestUtils.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Units.hpp>
#include <evolution/core/base/UserConfig.hpp>
#include <evolution/core/base/XmlConfigSource.hpp>
