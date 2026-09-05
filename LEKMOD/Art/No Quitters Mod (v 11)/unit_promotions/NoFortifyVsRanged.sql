-- Soft fortify vs ranged: DB column (promotion + free grants load after)
ALTER TABLE UnitPromotions ADD NoFortifyVsRanged INTEGER DEFAULT 0;
