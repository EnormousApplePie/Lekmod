-- Final production cost modifier after everything is calculated (used to give discounts for settlers)
ALTER TABLE Units ADD FinalProductionCostModifier INTEGER DEFAULT 0;