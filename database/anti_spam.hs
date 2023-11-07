{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE DeriveAnyClass #-}
{-# LANGUAGE DeriveGeneric #-}

import Data.Aeson
import qualified Data.ByteString.Lazy as B
import GHC.Generics

data Channel = Channel
    { channelId :: Int
    , channelName :: String
    } deriving (Show, Generic, FromJSON)

data Category = Category
    { categoryId :: Int
    , categoryName :: String
    } deriving (Show, Generic, FromJSON)

data User = User
    { userId :: Int
    , userName :: String
    , nameId :: String
    } deriving (Show, Generic, FromJSON)

data Message = Message
    { createdAt :: String
    , content :: String
    , hash :: String
    , msgLength :: Int
    } deriving (Show, Generic, FromJSON)

data JsonData = JsonData
    { channel :: Channel
    , category :: Category
    , user :: User
    , message :: Message
    } deriving (Show, Generic, FromJSON)

parseJSONFile :: FilePath -> IO (Maybe JsonData)
parseJSONFile filePath = do
    fileContent <- B.readFile filePath
    return (decode fileContent)

main :: IO ()
main = do
    result <- parseJSONFile "./message_log.json"
    case result of
        Just jsonData -> putStrLn $ "Parsed JSON: " ++ show jsonData
        Nothing -> putStrLn "Failed to parse JSON file."
