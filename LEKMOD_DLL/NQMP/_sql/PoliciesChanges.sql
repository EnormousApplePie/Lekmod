-----------------------------------------
-- *** IDEOLOGY UNIVERSAL CHANGES *** ---
-----------------------------------------
-- Number of extra spies granted from the policy
ALTER TABLE Policies ADD NumExtraSpies INTEGER DEFAULT 0;

--------------------------------
-- *** AUTOCRACY CHANGES *** ---
--------------------------------
-- Influence gained when tributing gold from a city state (instead of losing it)
ALTER TABLE Policies ADD MinorFriendshipGainBullyGoldSuccess INTEGER DEFAULT 0;
-- Flat change to % chance for all city state coups
ALTER TABLE Policies ADD CoupChanceModifier INTEGER DEFAULT 0;
-- (really ugly code) Gives a free Great Artist whenever you get a Great General if you have this policy
ALTER TABLE Policies ADD IsWarHero BOOLEAN DEFAULT false;
-- if true, puppet cities will not increase the science cost of technologies
ALTER TABLE Policies ADD IgnorePuppetsForResearchCosts BOOLEAN DEFAULT false;
-- if true, allows purchasing units in puppet cities
ALTER TABLE Policies ADD AllowPuppetPurchasing BOOLEAN DEFAULT false;


------------------------------
-- *** FREEDOM CHANGES *** ---
------------------------------
-- Influence modifier (percentage) when conducting trade missions (great merchant)
ALTER TABLE Policies ADD TradeMissionInfluenceModifier INTEGER DEFAULT 0;
-- Science gained whenever a great person is born
ALTER TABLE Policies ADD SciencePerGreatPersonBorn INTEGER DEFAULT 0;
-- Influence boost with all known city states whenever a great person is born
ALTER TABLE Policies ADD InfluenceBoostPerGreatPersonBorn INTEGER DEFAULT 0;
-- Percentage of trade mission (great merchant) gold converted to immediate flat tourism (like futurism, but on trade mission)
ALTER TABLE Policies ADD TourismFromTradeMissions INTEGER DEFAULT 0;
-- Percentage of empire's positive gold per turn income that is converted into science per turn
ALTER TABLE Policies ADD GoldToScience INTEGER DEFAULT 0;
-- Happiness gained per great person tile improved in borders
ALTER TABLE Policies ADD HappinessFromGreatImprovements INTEGER DEFAULT 0;
-- Prevents city states from declaring war if you are at least friends and have this policy
ALTER TABLE Policies ADD NoMinorDOWIfFriends BOOLEAN DEFAULT false;

----------------------------
-- *** ORDER CHANGES *** ---
----------------------------
-- percentage reduction in the increased science cost per city (5%) ... ex: "-20" here = -20% which is (5% * 0.8 = 4%)
ALTER TABLE Policies ADD NumCitiesResearchCostDiscount INTEGER DEFAULT 0;
-- flat tourism boost in each city
ALTER TABLE Policies ADD TourismPerCity INTEGER DEFAULT 0;
-- flat production boost from internal trade routes (both food and production routes)
ALTER TABLE Policies ADD InternalTradeRouteProductionYieldChange INTEGER DEFAULT 0;
-- toggle that disables resistance time in conquered cities if this policy is active
ALTER TABLE Policies ADD DisablesResistanceTime BOOLEAN DEFAULT false;
-- percentage modifier to unhappiness applied via ideological pressure from other civs (ex: -75 = only 25% of normal unhappiness)
ALTER TABLE Policies ADD IdeologyPressureUnhappinessModifier INTEGER DEFAULT 0;
-- combat bonus when fighting civs with fewer cities (basically opposite of Ethiopia UA)
ALTER TABLE Policies ADD CombatBonusVsSmallerCiv INTEGER DEFAULT 0;
-- percentage of empire's production output converted to bonus % for great musician points
ALTER TABLE Policies ADD ProductionToGreatMusiciansModifier INTEGER DEFAULT 0;
-- flat happiness gain from a rail connection (similar to road connection in liberty)
ALTER TABLE Policies ADD HappinessPerRailConnection INTEGER DEFAULT 0;
-- Bonus % production from having a rail connection (additive with base bonus)
ALTER TABLE Policies ADD RailConnectionProductionModifier INTEGER DEFAULT 0;
-- if true, training military land units produces 2 units instead of 1
ALTER TABLE Policies ADD IsDoubleTrainedMilitaryLandUnits BOOLEAN DEFAULT false;
