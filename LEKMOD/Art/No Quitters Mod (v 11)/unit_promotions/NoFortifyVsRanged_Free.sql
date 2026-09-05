-- Grant Exposed to Barrage to land ranged combat classes (after promo XML)
INSERT INTO Unit_FreePromotions (UnitType, PromotionType)
SELECT u.Type, 'PROMOTION_NO_FORTIFY_VS_RANGED'
FROM Units u
WHERE u.CombatClass IN (
	'UNITCOMBAT_ARCHER',
	'UNITCOMBAT_SIEGE',
	'UNITCOMBAT_MOUNTED_RANGED'
)
AND NOT EXISTS (
	SELECT 1 FROM Unit_FreePromotions f
	WHERE f.UnitType = u.Type AND f.PromotionType = 'PROMOTION_NO_FORTIFY_VS_RANGED'
);
