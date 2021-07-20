-- Insert SQL Rules Here 
ALTER TABLE Beliefs ADD FlatFaithPerCitizenBorn INTEGER DEFAULT 0;
ALTER TABLE Beliefs ADD AllowsFaithGiftsToMinors BOOLEAN DEFAULT false;