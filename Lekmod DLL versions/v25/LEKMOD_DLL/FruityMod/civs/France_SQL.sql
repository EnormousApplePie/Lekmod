-- Insert SQL Rules Here 
ALTER TABLE Traits ADD EarnsGreatPersonOnSlotOrGuild BOOLEAN DEFAULT false;
ALTER TABLE Buildings ADD GrantsFreeCulturalGreatPersonWithTrait BOOLEAN DEFAULT false;