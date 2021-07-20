--------------------------------
-- *** AUTOCRACY CHANGES *** ---
--------------------------------
-- Bonus combat strength when attacking in foreign lands during golden ages
ALTER TABLE UnitPromotions ADD GoldenAgeForeignAttackBonus INTEGER DEFAULT 0;
-- Bonus combat strength near friendly CS or CS that were conquered by another civ
ALTER TABLE UnitPromotions ADD CombatStrengthNearFriendlyMinor INTEGER DEFAULT 0;