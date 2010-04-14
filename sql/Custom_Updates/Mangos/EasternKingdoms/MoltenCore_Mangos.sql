/* MOLTEN CORE */
Update `gameobject` set `spawntimesecs` = -604800 where `id` = 179703;
-- Ragnaros shouldn't be spawned
update `creature` set `spawnMask` = 0 where `id` in (11502);
Update `areatrigger_teleport` set `required_quest_done` = 0 where `id` in (2886, 3529, 3528);

/* LOOT CHESTS */
update gameobject_template set faction = 0, data15 = 1 where entry in 
-- Halls of Stone
(190586, 193996, 
-- Culling of Stratholme
190663, 193597, 
-- Naxxramas
181366, 193426,
-- Hellfire Rampants
185168, 185169,
-- Molten Core
179703,
-- Halls of Reflection
202212, 201710, 202337, 202336,
-- Trial of the Champion
195709, 195710, 195374, 195375, 195323, 195324,
-- Trial of the Crusader
195631, 195632, 195633, 195635, 195665, 195666, 195667, 195668, 195669, 195670, 195671, 195672,
-- Ulduar
195046, 195047, 194307, 194308, 194200, 194201, 194312, 194314, 194326, 194331, 194749, 194956, 194822, 194821);