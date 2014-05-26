read.named.file <- function(fname, pilot){
  d <- read.csv(fname, header=TRUE, sep=",", stringsAsFactors=FALSE)
  d$pilot <- pilot
  return(d)
}

# read tx.omnet
d1 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/Joe1-GK.txO", "Joe1")
d2 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/Pedo-GK.txO", "Pedo")
d3 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/jJE--GK.txO", "-JE-")
tx.omnet <- rbind(d1, d2, d3)
rm(d1, d2, d3)

# read rx.omnet
d1 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/Joe1-GK.rxO", "Joe1")
d2 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/Pedo-GK.rxO", "Pedo")
d3 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/results/20131104/jJE--GK.rxO", "-JE-")
rx.omnet <- rbind(d1, d2, d3)
rm(d1, d2, d3)

# read tx.meas
d1 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/Joe1-GK.tx", "Joe1")
d2 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/Pedo-GK.tx", "Pedo")
d3 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/jJE--GK.tx", "-JE-")
tx.meas <- rbind(d1, d2, d3)
rm(d1, d2, d3)

# read rx.meas
d1 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/Joe1-GK.rx", "Joe1")
d2 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/Pedo-GK.rx", "Pedo")
d3 <- read.named.file("/Users/eckert/Projects/OMNeTpp/workspace/SkyNet/LogFiles/20131104/jJE--GK.rx", "-JE-")
rx.meas <- rbind(d1, d2, d3)
rm(d1, d2, d3)

# merge into txrx.omnet
txrx.omnet <- merge(tx.omnet, rx.omnet, by=c("PaketID"), suffixes=c(".tx", ".rx"), all=T)
rm(rx.omnet, tx.omnet)

# merge into txrx.meas
txrx.meas <- merge(tx.meas, rx.meas, by=c("PaketID"), suffixes=c(".tx", ".rx"), all=T)
rm(rx.meas, tx.meas)

# merge into txrx
txrx <- merge(txrx.omnet, txrx.meas, by=c("PaketID", "pilot.tx", "pilot.rx"), suffixes=c(".omnet", ".meas"), all=T)

# subset
d <- subset(txrx, select=c("PaketID", "pilot.tx", "pilot.rx", "Rssi.omnet", "Rssi.meas"))

# Are measured and modeled altitudes the same?
plot(txrx$Altitude.tx.omnet, txrx$Altitude.tx.meas, pch=".", cex=3)

# Hypothesis Test: do both have the same mean?
r <- t.test(txrx$Rssi.meas, txrx$Rssi.omnet, paired=T)
print(r)

require(ggplot2)
p <- ggplot(txrx) + geom_point(aes(x=Altitude.tx.omnet, y=Altitude.tx.meas)) + facet_grid(pilot.tx ~ pilot.rx)
print(p)

p <- ggplot(txrx) + geom_density(aes(x=Rssi.meas)) + facet_grid(pilot.tx ~ pilot.rx)
print(p)
