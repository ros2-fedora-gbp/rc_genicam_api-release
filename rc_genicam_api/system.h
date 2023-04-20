/*
 * This file is part of the rc_genicam_api package.
 *
 * Copyright (c) 2017 Roboception GmbH
 * All rights reserved
 *
 * Author: Heiko Hirschmueller
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RC_GENICAM_API_SYSTEM
#define RC_GENICAM_API_SYSTEM

#include <GenApi/GenApi.h>

#include <memory>
#include <vector>
#include <mutex>

namespace rcg
{

class GenTLWrapper;
class Interface;
class CPort;

/**
  The system class encapsulates a Genicam transport layer.

  NOTE: A GenTLException is thrown in case of a severe error.
*/

class System : public std::enable_shared_from_this<System>
{
  public:

    ~System();

    /**
      Set the path for finding GenTL producers on the file system and the name
      of a producer that should be ignored.

      NODE: This function must be called before the first call to getSystems()
      or after calling clearSystems().

      If this function is not called before calling getSystems() for the first
      time, then the path is taken from the environment variable
      GENICAM_GENTL32_PATH or GENICAM_GENTL64_PATH (depending on the
      compilation with 32 or 64 bit). If that variable is empty or not defined,
      then the install directory of the GenTL producers that are included in
      the rc_genicam_api are used.

      @param path   List of GenTL producers with absolute path or directories
                    with producers (i.e. with suffix .cti). The files or
                    directories are separated by ';' under Windows or ':' under
                    Linux. The parameter can be 0 or "" for using the install
                    directory of the GenTL producers that are included in the
                    rc_genicam_api.
      @param ignore Name of producer (i.e. file name without path) that is
                    ignored. This parameter can be 0.
      @return       False if either setSystemsPath() or setSystems() was called
                    before.
    */

    static bool setSystemsPath(const char *path, const char *ignore);


    /**
      This function creates systems for all producers that can be found. See
      also setSystemPath() for and explanation where the producers are sought.

      NOTE: This is the only method that can instantiate System objects.

      @return List of available systems.
    */

    static std::vector<std::shared_ptr<System> > getSystems();

    /**
      Clears the internal list of systems. This may be called before exit so
      that all resources are cleaned before leaving the main function.
    */

    static void clearSystems();

    /**
      Get file name from which this system was created.

      @return File name.
    */

    const std::string &getFilename() const;

    /**
      Opens the system for working with it. The system may be opened multiple
      times. However, for each open(), the close() method must be called as
      well.
    */

    void open();

    /**
      Closes the system. Each call of open() must be followed by a call to
      close() at some point in time.
    */

    void close();

    /**
      Returns the currently available interfaces.

      NOTE: open() must be called before calling this method.

      @return List of interfaces.
    */

    std::vector<std::shared_ptr<Interface> > getInterfaces();

    /**
      Returns the ID of the GenTL provider.

      @return ID.
    */

    std::string getID();

    /**
      Returns the vendor name of the GenTL provider.

      @return Vendor name.
    */

    std::string getVendor();

    /**
      Returns the model of the GenTL provider.

      @return Model.
    */

    std::string getModel();

    /**
      Returns the version of the GenTL provider.

      @return Version.
    */

    std::string getVersion();

    /**
      Returns the transport layer type of the GenTL provider.

      @return Transport layer type.
    */

    std::string getTLType();

    /**
      Returns the file name of the GenTL provider.

      @return File name.
    */

    std::string getName();

    /**
      Returns the full path name of the GenTL provider.

      @return Full path name.
    */

    std::string getPathname();

    /**
      Returns the display name of the GenTL provider.

      @return Display name.
    */

    std::string getDisplayName();

    /**
      Returns the character encoding.

      @return True for ASCII, false for UTF8.
    */

    bool isCharEncodingASCII();

    /**
      Returns the major version number.

      @return Major version number.
    */

    int getMajorVersion();

    /**
      Returns the minor version number.

      @return Minor version number.
    */

    int getMinorVersion();

    /**
      Returns the node map of this object.

      NOTE: open() must be called before calling this method. The returned
      pointer remains valid until close() of this object is called.

      @return Node map of this object.
    */

    std::shared_ptr<GenApi::CNodeMapRef> getNodeMap();

    /**
      Get internal handle of open transport layer.

      @return Internal handle.
    */

    void *getHandle() const;

  private:

    void clearInterfaces(); // Needed for ENUM-WORKAROUND

    System(const std::string &_filename);
    System(class System &); // forbidden
    System &operator=(const System &); // forbidden

    std::string filename;
    std::shared_ptr<const GenTLWrapper> gentl;

    std::recursive_mutex mtx;

    int n_open;
    void *tl;

    std::shared_ptr<CPort> cport;
    std::shared_ptr<GenApi::CNodeMapRef> nodemap;

    // Changed from weak_ptr to shared_ptr as part of ENUM-WORKAROUND
    std::vector<std::shared_ptr<Interface> > ilist;
};

}

#endif
