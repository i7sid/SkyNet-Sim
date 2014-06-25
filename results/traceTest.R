setwd("~/Projects/OMNeTpp/workspace/SkyNet/results")

minTime = 4100
maxTime = 4600

data <- read.delim("trace0.csv", header=TRUE, sep=",", stringsAsFactors=FALSE)
logF <- read.delim("../LogFiles/20131104/jJE--GK.tx", header=TRUE, sep=",", stringsAsFactors=FALSE)
#logF$SystemTime <- (logF$SystemTime-100) / 1000
logF$SystemTime <- (logF$TransmittTime-100) / 1000
dataLog <- merge(data, logF, by.x = "simtime", by.y= "SystemTime", suffixes=c(".data", ".log"))


#data <- subset(data, simtime <= maxTime)
#data <- subset(data, simtime >= minTime)

#relX <- data$targetX - data$currentX
#relY <- data$targetY - data$currentY
#relZ <- data$targetZ - data$currentZ

relX <- dataLog$Right - dataLog$currentX
relY <- dataLog$Height - dataLog$currentY
relZ <- dataLog$Altitude - dataLog$currentZ



plot(dataLog$simtime,relX, type='p', col="red", ylim=c(min(relX,relY,relZ),max(relX,relY,relZ)))
points(dataLog$simtime,relY, col="green")
points(dataLog$simtime,relZ, col="blue")