# This script configures PyQicsTable and generates the Makefiles.
#
# Copyright (c) 2004 - 2014
# 	Integrated Computer Solutions, Inc. <info@ics.com>
# 
# This file is part of PyQicsTable.
# 
# This copy of PyQicsTable is licensed for use under the terms of the
# PyQicsTable Commercial License Agreement version ICS-2.6.  See the file
# LICENSE for more details.
# 
# PyQicsTable is supplied WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


import sys
import os
import string
import glob
import getopt
import shutil
import py_compile

import sipconfig
# Where we must get a pyqtconfig module
from PyQt4 import pyqtconfig


# Initialise the globals.
pyqicstable_version = 0x030000
pyqicstable_version_str = "3.0.0"

sip_min_v3_version = 0x030b00
sip_min_v4_version = 0x040100
qt_min_version = 0x040200

qicstable_sip_flags = []
qicstable_version = None
qicstable_define = "CREATE_OBJS_WITH_QICSTABLE"

# Get the PyQt configuration.
pyqtcfg = pyqtconfig.Configuration()

if pyqtcfg.qt_version == 0:
    sipconfig.error("SIP has been built with Qt support disabled.")

# Command line options.
opt_pyqicstablemoddir = pyqtcfg.default_mod_dir
opt_pyqicstablesipdir = pyqtcfg.default_sip_dir
opt_qicstableincdir = "../../include" #pyqtcfg.qt_inc_dir
opt_qicstablelibdir = "../../lib" #pyqtcfg.qt_lib_dir
opt_static = 0
opt_debug = 0
opt_concat = 0
opt_split = 1
opt_releasegil = 0
opt_tracing = 0
opt_verbose = 0
opt_qicsTableDebug = 0


def usage(rcode = 2):
    """Display a usage message and exit.

    rcode is the return code passed back to the calling process.
    """
    print "Usage:"
    print "    python configure.py [-h] [-c] [-d dir] [-g] [-j #] [-k] [-n dir] [-o dir] [-r] [-u] [-v dir] [-w] [-td]"
    print "where:"
    print "    -h      display this help message"
    print "    -c      concatenate each module's C/C++ source files"
    print "    -d dir  where the PyQicsTable modules will be installed [default %s]" % opt_pyqicstablemoddir
    print "    -g      always release the GIL (SIP v3.x behaviour)"
    print "    -j #    split the concatenated C++ source files into # pieces [default 1]"
    print "    -k      build the PyQicsTable modules as static libraries"
    print "    -n dir  the directory containing the QicsTable header files [default %s]" % opt_qicstableincdir
    print "    -o dir  the directory containing the QicsTable library [default %s]" % opt_qicstablelibdir
    print "    -r      generate code with tracing enabled [default disabled]"
    print "    -u      build with debugging symbols"
    print "    -v dir  where the PyQicsTable .sip files will be installed [default %s]" % opt_pyqicstablesipdir
    print "    -w      don't suppress compiler output during configuration"
    print "    -q      link debug version of QicsTable"

    sys.exit(rcode)


def inform_user():
    """Tell the user the option values that are going to be used.
    """
    sipconfig.inform("The PyQicsTable modules will be installed in %s." % opt_pyqicstablemoddir)
    sipconfig.inform("The PyQicsTable .sip files will be installed in %s." % opt_pyqicstablesipdir)


def create_config(module, template):
    """Create the PyQicsTable configuration module so that it can be imported
    by build scripts.

    module is the module file name.
    template is the template file name.
    """
    sipconfig.inform("Creating %s..." % module)

    content = {
        "pyqicstable_version":              pyqicstable_version,
        "pyqicstable_version_str":          pyqicstable_version_str,
        "pyqicstable_mod_dir":              opt_pyqicstablemoddir,
        "pyqicstable_sip_dir":              opt_pyqicstablesipdir,
        "pyqicstable_qicstable_sip_flags":  qicstable_sip_flags,

        # These are internal.
        "_pyqicstable_defines":             qicstable_define,
        "_pyqicstable_inc_dir":             opt_qicstableincdir,
        "_pyqicstable_lib_dir":             opt_qicstablelibdir
    }

    sipconfig.create_config_module(module, template, content)


def check_qicstable():
    """See if QicsTable can be found and what its version is.
    """
    # Find the QicsTable header files.
    nspace = os.path.join(opt_qicstableincdir, "QicsNamespace.h")

    if os.access(nspace, os.F_OK):
        # Get the version number string.
        _, vstr = sipconfig.read_version(nspace, "QicsTable", None, "QICSTABLE_VERSION")

        # Extract the version number and hope that the format is consistent.
        try:
            vlist = string.split(vstr)
            maj, min, bug = string.split(vlist[1], ".")
            versnr = (int(maj) << 16) | (int(min) << 8) | int(bug)
            vstr = string.join(vlist[1:])
        except:
            raise ValueError, "Unable to extract version number from QICSTABLE_VERSION in QicsNamespace.h"

        if glob.glob(os.path.join(opt_qicstablelibdir, "*qicstable*")):
            global qicstable_version
            qicstable_version = versnr

            sipconfig.inform("QicsTable %s is being used." % vstr)
        else:
            sipconfig.inform("The QicsTable library could not be found in %s and so the qicstable module will not be built. If QicsTable is installed then use the -o argument to explicitly specify the correct directory." % opt_qicstablelibdir)
    else:
        sipconfig.inform("QicsNamespace.h could not be found in %s and so the qicstable module will not be built. If QicsTable is installed then use the -n argument to explicitly specify the correct directory." % opt_qicstableincdir)


def set_sip_flags():
    """Set the SIP platform, version and feature flags.
    """
    global qicstable_sip_flags
    # For new version of PyQt (same 4 >) we must use pyqtcfg.pyqt_sip_flags but not pyqtcfg.pyqt_qt_sip_flags
    qicstable_sip_flags = string.split(pyqtcfg.pyqt_sip_flags)

    # Handle the QicsTable version tag.  At the moment QicsTable doesn't have
    # multiple versions.  When it does, add them and the %Timeline tags to this
    # disctionary.
    qicstable_tags = {
    }

    if qicstable_tags:
        qicstable_sip_flags.append("-t")
        qicstable_sip_flags.append(sipconfig.version_to_sip_tag(qicstable_version, qicstable_tags, "QicsTable"))


def generate_code(mname, imports=None, extra_cflags=None, extra_cxxflags="-I%s/QtCore/ -I%s/QtXml/ -I%s/QtGui/ -I%s -I../../../include -L../../../lib" % (pyqtcfg.qt_inc_dir, pyqtcfg.qt_inc_dir, pyqtcfg.qt_inc_dir, pyqtcfg.qt_inc_dir), extra_define=None, extra_include_dir=None, extra_lflags=None, extra_lib_dir=None, extra_lib=None, sip_flags=None):
    """Generate the code for a module.

    mname is the name of the module.
    imports is the list of PyQt modules that this one %Imports.
    extra_cflags is a string containing additional C compiler flags.
    extra_cxxflags is a string containing additional C++ compiler flags.
    extra_define is a name to add to the list of preprocessor defines.
    extra_include_dir is the name of a directory to add to the list of include
    directories.
    extra_lflags is a string containing additional linker flags.
    extra_lib_dir is the name of a directory to add to the list of library
    directories.
    extra_lib is the name of an extra library to add to the list of libraries.
    sip_flags is the list of sip flags to use instead of the defaults.
    """
    sipconfig.inform("Generating the C++ source for the %s module..." % mname)

    try:
        shutil.rmtree(mname)
    except:
        pass

    try:
        os.mkdir(mname)
    except:
        sipconfig.error("Unable to create the %s directory." % mname)

    # Build the SIP command line.
    argv = [pyqtcfg.sip_bin]

    if sip_flags:
        argv.extend(sip_flags)

    if opt_concat:
        argv.append("-j")
        argv.append(str(opt_split))

    if opt_tracing:
        argv.append("-r")

    if opt_releasegil:
        argv.append("-g")

    argv.append("-c")
    argv.append(mname)

    buildfile = os.path.join(mname, mname + ".sbf")
    argv.append("-b")
    argv.append(buildfile)

    argv.append("-I")
    argv.append(pyqtcfg.pyqt_sip_dir)

    # SIP assumes POSIX style path separators.
    argv.append(string.join(["sip", mname, mname + "mod.sip"], "/"))

    os.system(string.join(argv))

    # Check the result.
    if not os.access(buildfile, os.F_OK):
        sipconfig.error("Unable to create the C++ code.")

    # Compile the Python stub.
    if pyqtcfg.sip_version < 0x040000:
        sipconfig.inform("Compiling %s.py..." % mname)
        py_compile.compile(os.path.join(mname, mname + ".py"), os.path.join(mname, mname + ".pyc"))

    # Generate the Makefile.
    sipconfig.inform("Creating the Makefile for the %s module..." % mname)

    installs = []

    if pyqtcfg.sip_version >= 0x040000:
        warnings = 1
    else:
        warnings = 0
        installs.append([[mname + ".py", mname + ".pyc"], opt_pyqicstablemoddir])

    sipfiles = []

    for s in glob.glob("sip/" + mname + "/*.sip"):
        sipfiles.append(os.path.join("..", "sip", mname, os.path.basename(s)))

    installs.append([sipfiles, os.path.join(opt_pyqicstablesipdir, mname)])

    makefile = sipconfig.SIPModuleMakefile(
        configuration=pyqtcfg,
        build_file=mname + ".sbf",
        dir=mname,
        install_dir=opt_pyqicstablemoddir,
        installs=installs,
        qt=1,
        warnings=warnings,
        static=opt_static,
        debug=opt_debug
    )

    if extra_cflags:
        makefile.extra_cflags.append(extra_cflags)

    if extra_cxxflags:
        makefile.extra_cxxflags.append(extra_cxxflags)

    if extra_define:
        makefile.extra_defines.append(extra_define)

    if extra_include_dir:
        makefile.extra_include_dirs.append(extra_include_dir)

    if extra_lflags:
        makefile.extra_lflags.append(extra_lflags)

    if extra_lib_dir:
        makefile.extra_lib_dirs.append(extra_lib_dir)

    if extra_lib:
        makefile.extra_libs.append(extra_lib)

    if pyqtcfg.sip_version < 0x040000 and imports:
        # Inter-module links.
        for im in imports:
            makefile.extra_lib_dirs.insert(0, pyqtcfg.pyqt_mod_dir)
            makefile.extra_libs.insert(0, makefile.module_as_lib(im))

    makefile.generate()


def check_license():
    """Handle the validation of the PyQicsTable license.
    """
    try:
        import license
        ltype = license.LicenseType
        lname = license.LicenseName

        try:
            lfile = license.LicenseFile
        except AttributeError:
            lfile = None
    except ImportError:
        ltype = None

    if ltype is None:
        ltype = "GPL"
        lname = "GNU General Public License"
        lfile = None

    sipconfig.inform("This is the %s version of PyQicsTable %s (licensed under the %s) for Python %s on %s." % (ltype, pyqicstable_version_str, lname, string.split(sys.version)[0], sys.platform))

    # Common checks.
    if ltype == "GPL" and sys.platform == "win32":
        error("You cannot use the GPL version of PyQicsTable under Windows.")

    try:
        qted = pyqtcfg.qt_edition
    except AttributeError:
        qted = None

    if qted:
        if (qted == "free" and ltype != "GPL") or (qted != "free" and ltype == "GPL"):
            sipconfig.error("This version of PyQicsTable and the %s edition of Qt have incompatible licenses." % qted)

    # Confirm the license.
    print
    print "Type 'L' to view the license."
    print "Type 'yes' to accept the terms of the license."
    print "Type 'no' to decline the terms of the license."
    print

    while 1:
        try:
            resp = raw_input("Do you accept the terms of the license? ")
        except:
            resp = ""

        resp = string.lower(string.strip(resp))

        if resp == "yes":
            break

        if resp == "no":
            sys.exit(0)

        if resp == "l":
            os.system("more LICENSE")

    # If there should be a license file then check it is where it should be.
    if lfile:
        if os.access(os.path.join("sip", lfile), os.F_OK):
            sipconfig.inform("Found the license file %s." % lfile)
        else:
            sipconfig.error("Please copy the license file %s to the sip directory." % lfile)


def create_makefiles():
    """Create the additional Makefiles.
    """
    sipconfig.inform("Creating top level Makefile...")

    sipconfig.ParentMakefile(
        configuration=pyqtcfg,
        subdirs=["qicstable"],
        installs=("pyqicstableconfig.py", opt_pyqicstablemoddir)
    ).generate()


def main(argv):
    """Create the configuration module module.

    argv is the list of command line arguments.
    """
    try:
        optlist, args = getopt.getopt(argv[1:], "hcd:gj:kn:o:ruv:w:q")
    except getopt.GetoptError:
        usage()

    global opt_pyqicstablemoddir, opt_pyqicstablesipdir
    global opt_qicstableincdir, opt_qicstablelibdir
    global opt_static, opt_debug, opt_concat, opt_releasegil, opt_split
    global opt_tracing, opt_verbose, opt_qicsTableDebug

    for opt, arg in optlist:
        if opt == "-h":
            usage(0)
        elif opt == "-c":
            opt_concat = 1
        elif opt == "-d":
            opt_pyqicstablemoddir = arg
        elif opt == "-g":
            opt_releasegil = 1
        elif opt == "-j":
            try:
                opt_split = int(arg)
            except:
                usage()
        elif opt == "-k":
            opt_static = 1
        elif opt == "-n":
            opt_qicstableincdir = arg
        elif opt == "-o":
            opt_qicstablelibdir = arg
        elif opt == "-r":
            opt_tracing = 1
        elif opt == "-u":
            opt_debug = 1
        elif opt == "-v":
            opt_pyqicstablesipdir = arg
        elif opt == "-w":
            opt_verbose = 1
        elif opt == "-q":
            opt_qicsTableDebug = 1

    if args:
        usage()

    check_license()

    sipconfig.inform("SIP %s is being used." % pyqtcfg.sip_version_str)

    # Check SIP is new enough.
    if pyqtcfg.sip_version_str[:8] != "snapshot":
        minv = None

        if pyqtcfg.sip_version >= 0x040000:
            if pyqtcfg.sip_version < sip_min_v4_version:
                minv = sip_min_v4_version
        else:
            if pyqtcfg.sip_version < sip_min_v3_version:
                minv = sip_min_v3_version

        if minv:
            sipconfig.error("This version of PyQicsTable requires SIP v%s or later" % sipconfig.version_to_string(minv))

    # Check for QicsTable.
    check_qicstable()
	
    # If QicsTable wasn't found then there is no point carrying on.  An
    # appropriate error message will already have been displayed.
    if qicstable_version is None:
       sys.exit(1)

	# Set the SIP platform, version and feature flags.
    set_sip_flags()

    # Tell the user what's been found.
    inform_user()
    
    lib_dir = "../../../lib"
    if opt_qicstablelibdir != "../../lib":
        lib_dir = opt_qicstablelibdir

    # Generate the code.
    libName = None
    if os.name == "nt":
        if opt_qicsTableDebug == 1:
            libName = "qicstabled2"
        else:
            libName = "qicstable2"
    else:
        if opt_qicsTableDebug == 1:
            libName = "qicstable_debug"
        else:
            libName = "qicstable"

    generate_code("qicstable", imports=["PyQt4"], extra_define=qicstable_define, extra_include_dir=opt_qicstableincdir, extra_lib_dir=lib_dir, extra_lib=libName, sip_flags=qicstable_sip_flags)

    # Create the additional Makefiles.
    create_makefiles()

    # Install the configuration module.
    create_config("pyqicstableconfig.py", "pyqicstableconfig.py.in")


###############################################################################
# The script starts here.
###############################################################################

if __name__ == "__main__":
    try:
        main(sys.argv)
    except SystemExit:
        raise
    except:
        print \
"""An internal error occured.  Please report all the output from the program,
including the following traceback, to support@ics.com.
"""
        raise
