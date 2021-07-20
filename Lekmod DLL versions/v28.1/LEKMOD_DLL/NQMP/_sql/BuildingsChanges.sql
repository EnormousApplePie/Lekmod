-- amount of population this city gains immediately
ALTER TABLE Buildings ADD LocalPopulationChange INTEGER DEFAULT 0;
-- defense per citizen in the city (i.e. 1 pop = 1 defense, 2 pop = 2 defense, etc), writtin in 100's
ALTER TABLE Buildings ADD DefensePerCitizen INTEGER DEFAULT 0;
-- flat faith gain in the empire upon constructing this building
ALTER TABLE Buildings ADD FreeFlatFaith INTEGER DEFAULT 0;
-- HACK: Mali treasury does some very specific stuff in C++
ALTER TABLE Buildings ADD MaliTreasury BOOLEAN DEFAULT false;