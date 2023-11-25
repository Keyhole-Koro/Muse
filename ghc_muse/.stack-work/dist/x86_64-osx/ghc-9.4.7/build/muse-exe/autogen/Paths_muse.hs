{-# LANGUAGE CPP #-}
{-# LANGUAGE NoRebindableSyntax #-}
{-# OPTIONS_GHC -fno-warn-missing-import-lists #-}
{-# OPTIONS_GHC -w #-}
module Paths_muse (
    version,
    getBinDir, getLibDir, getDynLibDir, getDataDir, getLibexecDir,
    getDataFileName, getSysconfDir
  ) where


import qualified Control.Exception as Exception
import qualified Data.List as List
import Data.Version (Version(..))
import System.Environment (getEnv)
import Prelude


#if defined(VERSION_base)

#if MIN_VERSION_base(4,0,0)
catchIO :: IO a -> (Exception.IOException -> IO a) -> IO a
#else
catchIO :: IO a -> (Exception.Exception -> IO a) -> IO a
#endif

#else
catchIO :: IO a -> (Exception.IOException -> IO a) -> IO a
#endif
catchIO = Exception.catch

version :: Version
version = Version [0,1,0,0] []

getDataFileName :: FilePath -> IO FilePath
getDataFileName name = do
  dir <- getDataDir
  return (dir `joinFileName` name)

getBinDir, getLibDir, getDynLibDir, getDataDir, getLibexecDir, getSysconfDir :: IO FilePath



bindir, libdir, dynlibdir, datadir, libexecdir, sysconfdir :: FilePath
bindir     = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/bin"
libdir     = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/lib/x86_64-osx-ghc-9.4.7/muse-0.1.0.0-Fqk5vSJH1bxCtuIxTy6kJT-muse-exe"
dynlibdir  = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/lib/x86_64-osx-ghc-9.4.7"
datadir    = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/share/x86_64-osx-ghc-9.4.7/muse-0.1.0.0"
libexecdir = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/libexec/x86_64-osx-ghc-9.4.7/muse-0.1.0.0"
sysconfdir = "/Users/katsukawakiho/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Muse/ghc_muse/.stack-work/install/x86_64-osx/c58181ea96c444ccd66921a6613dfc6c6dcc7966256b9d2e617e2c6d2241f9ea/9.4.7/etc"

getBinDir     = catchIO (getEnv "muse_bindir")     (\_ -> return bindir)
getLibDir     = catchIO (getEnv "muse_libdir")     (\_ -> return libdir)
getDynLibDir  = catchIO (getEnv "muse_dynlibdir")  (\_ -> return dynlibdir)
getDataDir    = catchIO (getEnv "muse_datadir")    (\_ -> return datadir)
getLibexecDir = catchIO (getEnv "muse_libexecdir") (\_ -> return libexecdir)
getSysconfDir = catchIO (getEnv "muse_sysconfdir") (\_ -> return sysconfdir)




joinFileName :: String -> String -> FilePath
joinFileName ""  fname = fname
joinFileName "." fname = fname
joinFileName dir ""    = dir
joinFileName dir fname
  | isPathSeparator (List.last dir) = dir ++ fname
  | otherwise                       = dir ++ pathSeparator : fname

pathSeparator :: Char
pathSeparator = '/'

isPathSeparator :: Char -> Bool
isPathSeparator c = c == '/'
