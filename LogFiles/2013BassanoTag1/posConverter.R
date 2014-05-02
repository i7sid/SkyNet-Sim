#setwd("~/Projects/OMNeTpp/workspace/SkyNet/LogFiles/2013BassanoTag1/")

# FILEz
getAvailableLogs <- function()
{
  list <- list.files(pattern = "tx")
  return(substr(list, 1, 27))
}

getDevice <- function()
{
  filez <- substr(getAvailableLogs(), 21,27)
  return((unique(filez)))
}

readFilez <- function(filter)
{
  mlist <- list.files(pattern = filter)
  data <- do.call("rbind", lapply(mlist, read.csv, header = TRUE))  
  return(data)
}

loadFilez <-function(device)
{
  filtername <<- c("tx", "rx", "pos")
  filter <- sapply(filtername, function(filler) paste("(", device, ").(", filler, ")", sep="")) 
  dev <- lapply(filter, readFilez)
  names(dev) <- filtername
  return(dev)                                                 
}

writeFilez <- function(db)
{
  for(dev in names(db))   
  {
   for(file in filtername)
   {
     write.csv(db[[dev]][[file]], file = paste(dev, "-GK.", file, sep = ""), row.names=FALSE, quote=FALSE)
   }
  }
}


##################
## import database
##################
getDB <- function()
{
  db <- lapply(getDevice(), loadFilez)
  names(db) <- getDevice()
  
  return(db)
}


### conversion
convert2GK <-function (database, device)
{
  for(tab in filtername)
  {
    posLatLon <- cbind(database[[device]][[tab]][,"Longitude"],database[[device]][[tab]][,"Latitude"])  
    write.table(posLatLon,file="latLon.csv",sep=" ",col.names=FALSE, row.names=FALSE)
    system(paste("cs2cs +proj=latlong +datum=WGS84 +to +init=epsg:31467 < latLon.csv > latLonGKraw.csv"))
    system(paste("awk 'BEGIN{FS=\"[ ]|[\t]|[,]\";OFS=\",\"};{$1=$1};{print $0}' < latLonGKraw.csv  > latLonGK.csv")) 
    posLatLonGK <- read.csv(file="latLonGK.csv", sep=",", header=F)
    system(paste("rm latLonGKraw.csv latLon.csv latLonGK.csv"))
    names(posLatLonGK) <- c("Right","Hight","ignor")
    
    database[[device]][[tab]][,"Longitude"] <- posLatLonGK[,"Right"]
    database[[device]][[tab]][,"Latitude"]  <- posLatLonGK[,"Hight"]
    colnames(database[[device]][[tab]])[which(colnames(database[[device]][[tab]]) == "Longitude")] <- "Right"
    colnames(database[[device]][[tab]])[which(colnames(database[[device]][[tab]]) == "Latitude")] <- "Hight"
  }
  
  return(database)
}

offsetCorr <-function (database, device, minHight, minRight, minTime)
{
  for(tab in filtername)
  {
    database[[device]][[tab]][,"Hight"] <- database[[device]][[tab]][,"Hight"] - minHight 
    database[[device]][[tab]][,"Right"] <- database[[device]][[tab]][,"Right"] - minRight 
  }

  database[[device]][["pos"]][,"SystemTime"] <- database[[device]][["pos"]][,"SystemTime"] - minTime
  database[[device]][["tx"]][,"TransmittTime"] <- database[[device]][["tx"]][,"TransmittTime"] - minTime
  database[[device]][["rx"]][,"ReceiveTime"] <- database[[device]][["rx"]][,"ReceiveTime"] - minTime
  
  return(database)
}

converteLogs <- function(path)
{
  setwd(path)
  options(digits=15)
  db <- getDB()
  
  minHight <- .Machine$integer.max
  maxHight <- -.Machine$integer.max
  
  minRight <- .Machine$integer.max
  maxRight <- -.Machine$integer.max

  maxAltitude <- -.Machine$integer.max

  minTime <- Inf
  
  for(dev in names(db))
  {
    db <- convert2GK(db, dev)
    maxHight <- max(db[[dev]][["pos"]][,"Hight"], maxHight)
    minHight <- min(db[[dev]][["pos"]][,"Hight"], minHight)
    maxRight <- max(db[[dev]][["pos"]][,"Right"], maxRight)
    minRight <- min(db[[dev]][["pos"]][,"Right"], minRight)
    maxAltitude <- max(db[[dev]][["pos"]][,"Altitude"], maxAltitude)
    minTime <- min(db[[dev]][["pos"]][,"SystemTime"],db[[dev]][["tx"]][,"TransmittTime"],db[[dev]][["rx"]][,"ReceiveTime"], minTime)
  }
  for(dev in names(db))
  {
    db <- offsetCorr(db, dev, minHight, minRight, minTime)
  }

  cat("# Omnet parameters:\n")
  cat("# Simtime offset: ", minTime, "s\n")
  cat("**.numNodes = ", length(names(db)), "\n")
  cat("**.playgroundSizeX = " , maxRight - minRight, "m\n")
  cat("**.playgroundSizeY = " , maxHight - minHight, "m\n")
  cat("**.playgroundSizeZ = " , maxAltitude, "m\n")
  
  
  writeFilez(db)
}

