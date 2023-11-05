{-# LANGUAGE OverloadedStrings #-}

import Data.Aeson
import qualified Data.ByteString.Lazy as B

data Channel = Channel
    { channelId :: Int
    , channelName :: String
    } deriving (Show, FromJSON)

data Category = Category
    { categoryId :: Int
    , categoryName :: String
    } deriving (Show, FromJSON)

data User = User
    { userId :: Int
    , userName :: String
    , nameId :: String
    } deriving (Show, FromJSON)

data Message = Message
    { createdAt :: String
    , content :: String
    , hash :: String
    , msgLength :: Int  -- Renamed 'length' to 'msgLength'
    } deriving (Show, FromJSON)

data JsonData = JsonData
    { channel :: Channel
    , category :: Category
    , user :: User
    , message :: Message
    } deriving (Show, FromJSON)

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
