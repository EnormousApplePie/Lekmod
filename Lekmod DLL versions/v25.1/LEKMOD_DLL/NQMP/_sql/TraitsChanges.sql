-- number of consecutive turns after which city state allies refuse gold gifts from other civilizations
ALTER TABLE Traits ADD NumTurnsBeforeMinorAlliesRefuseBribes INTEGER DEFAULT 0;
-- cargo ships are immune to being plundered
ALTER TABLE Traits ADD SeaTradeRoutesArePlunderImmune BOOLEAN DEFAULT false;
-- during Golden Ages, faith yield increase on any tile making at least 1 gold 
ALTER TABLE Traits ADD GoldenAgeTileBonusFaith INTEGER DEFAULT 0;