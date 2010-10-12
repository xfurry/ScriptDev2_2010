-- Update the graveyard link for Toc until some bugs are fixed
-- original graveyard: id = 1478; toc area = 4722 = 12s time/dist
-- id = 1397 northred icecrown glacyer = 30s time/dist
-- id = 1396 vyrkyl central = 80s time/dist - better
update game_graveyard_zone set id = 1478 where ghost_zone = 4722;
