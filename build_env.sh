# Check for LD_LIBRARY_PATH being set, which can break SDK and generally is a bad practice
# http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html#AEN80
# http://xahlee.info/UnixResource_dir/_/ldpath.html
# Only disable this check if you are absolutely know what you are doing!
if [ ! -z "$LD_LIBRARY_PATH" ]; then
    echo "Your environment is misconfigured, you probably need to 'unset LD_LIBRARY_PATH'"
    echo "but please check why this was set in the first place and that it's safe to unset."
    echo "The SDK will not operate correctly in most cases when LD_LIBRARY_PATH is set."
    echo "For more references see:"
    echo "  http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html#AEN80"
    echo "  http://xahlee.info/UnixResource_dir/_/ldpath.html"
    return 1
fi

export SUP_GRPCROOT=$HOME/.local
export PKG_CONFIG_SYSROOT_DIR=/:$SUP_GRPCROOT
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/share/pkgconfig:$SUP_GRPCROOT/usr/lib/pkgconfig:$SUP_GRPCROOT/usr/share/pkgconfig
export CXX=g++
