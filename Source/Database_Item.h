#ifndef _DATABASE_ITEM_H
#define _DATABASE_ITEM_H

#include "Database_Common.h"

//-- Shops
static SortedChoiceItem HADES_STRING_SHOP_NAME[] = {
	{ 0, L"Dali (Disc 1-4)" },
	{ 1, L"Lindblum (Disc 1)" },
	{ 2, L"Burmecia (Disc 1-4)" },
	{ 3, L"Summit Station (Disc 2)" },
	{ 4, L"Treno (Disc 2)" },
	{ 5, L"Cleyra (Disc 2)" },
	{ 6, L"Lindblum (Disc 2)" },
	{ 7, L"Fossile Roo (Disc 2)" },
	{ 8, L"Conde Petie (Disc 2-3)" },
	{ 9, L"Black Mage Village (Disc 2-3)" },
	{ 10, L"Madain Sari (Disc 2-4)" },
	{ 11, L"Alexandria (Disc 3-4)" },
	{ 12, L"Treno (Disc 3-4)" },
	{ 13, L"Lindblum (Disc 3-4)" },
	{ 14, L"Oeilvert (Disc 3)" },
	{ 15, L"Esto Gaza (Disc 3)" },
	{ 16, L"Ipsen's Castle (Disc 3-4)" },
	{ 17, L"Daguerreo (Disc 3)" },
	{ 18, L"Bran Bal (Disc 3)" },
	{ 19, L"Daguerreo (Disc 4)" },
	{ 20, L"Black Mage Village (Disc 4)" },
	{ 21, L"Esto Gaza (Disc 3)" },
	{ 22, L"Desert Palace (Disc 3)" },
	{ 23, L"Unused 1" },
	{ 24, L"Unused 2" },
	{ 25, L"Item Shop 1 (Disc 1-4)" },
	{ 26, L"Item Shop 2 (Disc 1-3)" },
	{ 27, L"Item Shop 3 (Disc 2-3)" },
	{ 28, L"Item Shop 4 (Disc 3-4)" },
	{ 29, L"Item Shop 5 (Disc 3-4)" },
	{ 30, L"Item Shop 6 (Disc 4)" },
	{ 31, L"Item Shop 7 (Zorn & Thorn)" },
	{ 32, L"Synth Shop: Lindblum (Disc 1-2)" },
	{ 33, L"Synth Shop: Treno (Disc 2)" },
	{ 34, L"Synth Shop: Lindblum (Disc 2)" },
	{ 35, L"Synth Shop: B. M. Village (Disc 2-3)" },
	{ 36, L"Synth Shop: Alex. Lind. Treno (Disc 3-4)" },
	{ 37, L"Synth Shop: Daguerreo (Disc 3-4)" },
	{ 38, L"Synth Shop: B. M. Village (Disc 4)" },
	{ 39, L"Synth Shop: Hades (Disc 4)" }
};

//-- Cards
static SortedChoiceItem HADES_STRING_DECK_NAME[] = {
	{ 0, L"Deck 0 - Alexandria..." }, // [Alexandria 1] Synthesist + Man (Alexandria 1) + Michelle (Alexandria 3) + Old Man (Treno)
	{ 1, L"Deck 1 - Alexandria" }, // [Alexandria 1] Mick + Synthesist's Wife (Alexandria 1)
	{ 2, L"Deck 2 - Alexandria..." }, // [Alexandria 1] Honorable Lord + Fish Man (Alexandria 1) + Young Nobleman (Treno)
	{ 3, L"Deck 3 - Alexandria..." }, // [Alexandria 1] Honorable Lady + Hippolady (Alexandria 1) + Noblewoman (Treno) + Tim (Lindblum 1)
	{ 4, L"Deck 4 - Alexandria..." }, // [Alexandria 1] Florist + Ashley (Alexandria 1) + Thomas (Lindblum 1)
	{ 5, L"Deck 5 - Alexandria..." }, // [Alexandria 1] Boatman + Maggie (Alexandria 1) + Rita (Lindblum 1)
	{ 6, L"Deck 6 - Alexandria..." }, // [Alexandria 1] Retired Boatman + Dante the Signmaker (Alexandria 1) + Nobleman (Treno) + Member n°56 (Lindblum 1)
	{ 7, L"Deck 7 - Alexandria..." }, // [Alexandria 1] Tom + Male Red Mage (Alexandria 1) + Adventurer (Treno) + Member n°29 (Lindblum 1)
	{ 8, L"Deck 8 - Alexandria" }, // [Alexandria 1] Ilia + Noble Lady (Alexandria 1)
	{ 9, L"Deck 9 - Alexandria" }, // [Alexandria 1] Noble Lord + Old Man (Alexandria 1)
	{ 10, L"Deck 10 - Alexandria..." }, // [Alexandria 1] Nikolai + Zenero (Alexandria 3) + Criminal (Treno)
	{ 11, L"Deck 11 - Alexandria..." }, // [Dali] Morrid + Fish Man (Alexandria 3) + Doug (Alexandria 1) + Female Adventurer (Treno)
	{ 12, L"Deck 12 - Alexandria..." }, // [Dali] Shopkeeper Eve + Shopkeeper (Alexandria 1)
	{ 13, L"Deck 13 - Alexandria" }, // [Alexandria 1] Ryan
	{ 14, L"Deck 14 - Alexandria" }, // [Alexandria 3] Benero + Hippaul (Alexandria 1)
	{ 15, L"Deck 15 - Alexandria..." }, // [Alexandria 3] Old Lady + Alleway Jack (Alexandria 1) + Father (Treno) + Gus (Lindblum 1) + Lilian (Lindblum 1)
	{ 16, L"Deck 16 - Lindblum..." }, // [Dali] Part-time Worker Mary + Card Freak Gon (Lindblum 1 & 3)
	{ 17, L"Deck 17 - Lindblum" }, // [Lindblum 1] Marsha
	{ 18, L"Deck 18 - Lindblum..." }, // [Alexandria 3] Genero + Old Margaret (Lindblum 1)
	{ 19, L"Deck 19 - Lindblum..." }, // [Alexandria 3] Hippolady + Drunk (Treno) + Female Red Mage (Lindblum 1)
	{ 20, L"Deck 20 - Lindblum" }, // [Lindblum 1] Wimpy Guy
	{ 21, L"Deck 21 - Lindblum" }, // [Lindblum 1] Lady with an Apron
	{ 22, L"Deck 22 - Lindblum..." }, // [Alexandria 3] Boatman + Yaup (Lindblum 1)
	{ 23, L"Deck 23 - Treno..." }, // [Alexandria 3] Man + Nobleman (Treno)
	{ 24, L"Deck 24 - Lindblum" }, // [Lindblum 1 & 3] Attendant
	{ 25, L"Deck 25 - Lindblum..." }, // [Chocobo Paradise] Yellow Chocobo + Marian (Lindblum 1)
	{ 26, L"Deck 26 - Lindblum..." }, // [Alexandria 3] Retired Boatman + Artist Michael (Lindblum 1 & 3)
	{ 27, L"Deck 27 - Lindblum..." }, // [Alexandria 3] Old Man + Noblewoman (Treno) + Pigeon Lover (Lindblum 1)
	{ 28, L"Deck 28 - South Gate" }, // [South Gate] Worker
	{ 29, L"Deck 29 - Treno" }, // [Treno] Natalie
	{ 30, L"Deck 30 - South Gate..." }, // [Alexandria 3] Tom + Shopkeeper (Treno) + Chef (South Gate)
	{ 31, L"Deck 31 - Treno..." }, // [Alexandria 3] Tour Guide + Thug (Treno)
	{ 32, L"Deck 32 - Treno" }, // [Treno] Carpenter
	{ 33, L"Deck 33 - Treno" }, // [Treno] Scholar
	{ 34, L"Deck 34 - Treno..." }, // [Dali] Part-time Worker Jeff + Onionmeister (Alexandria 3) + Shopkeeper (Treno)
	{ 35, L"Deck 35 - Treno..." }, // [Alexandria 3] Mick + Queen Stella (Treno)
	{ 36, L"Deck 36 - Treno" }, // [Treno] Servant
	{ 37, L"Deck 37 - Treno..." }, // [Chocobo Paradise] Blue Chocobo + Thug (Treno)
	{ 38, L"Deck 38 - Treno..." }, // [Alexandria 3] Ilia + Barkeep (Treno)
	{ 39, L"Deck 39 - Treno..." }, // [Alexandria 3] Doug + Mario (Treno)
	{ 40, L"Deck 40 - Treno" }, // [Treno] Waitress
	{ 41, L"Deck 41 - Treno..." }, // [Chocobo Paradise] Red Chocobo + Tot (Treno) + Dan (Cleyra)
	{ 42, L"Deck 42 - Treno..." }, // [Alexandria 3] Maggie + Thug (Treno) + Laerie (Cleyra)
	{ 43, L"Deck 43 - Alexandria" }, // [Alexandria 3] Honorable Lady
	{ 44, L"Deck 44 - Treno..." }, // [Treno] Thug (after throwing gils) + Jack (Cleyra)
	{ 45, L"Deck 45 - Treno..." }, // [Treno] Nobleman + Adam (Cleyra)
	{ 46, L"Deck 46 - Alexandria" }, // [Alexandria 3] Dante the Signmaker
	{ 47, L"Deck 47 - Treno..." }, // [Alexandria 3] Honorable Lord + Noblewoman (Treno)
	{ 48, L"Deck 48 - Treno" }, // [Treno] Girl
	{ 49, L"Deck 49 - Alexandria" }, // [Alexandria 3+] Boatman
	{ 50, L"Deck 50 - Mage Village..." }, // [Mage Village] Black Mage 24 + Retired Boatman (Alexandria 3+)
	{ 51, L"Deck 51 - Fossil Roo..." }, // [Mage Village] Black Mage 123 + Flower Girl (Alexandria) + Tom (Alexandria 3+) + Treasure Hunter (Fossil Roo)
	{ 52, L"Deck 52 - Fossil Roo..." }, // [Mage Village] Black Mage 234 + Flower Girl (Alexandria 3+) + Treasure Hunter (Fossil Roo)
	{ 53, L"Deck 53 - Condee Petie..." }, // [Mage Village] Black Mage 189 + John Fruitbringer (Condee Petie) + Weimar (Alexandria 3+)
	{ 54, L"Deck 54 - Condee Petie..." }, // [Mage Village] Black Mage 44 + Wendy Grocer (Condee Petie) + Old Lady (Alexandria 3+)
	{ 55, L"Deck 55 - Condee Petie..." }, // [Mage Village] Black Mage 192 + Kelley Fingerwaver (Condee Petie) + Synthesist (Alexandria 3+)
	{ 56, L"Deck 56 - Condee Petie..." }, // [Mage Village] Black Mage 56 + Shamis Gatekeeper (Condee Petie) + Ashley (Alexandria 3+)
	{ 57, L"Deck 57 - Condee Petie..." }, // [Mage Village] Black Mage 33 + Robert Dogherder (Condee Petie) + Tour Guide (Alexandria 3+)
	{ 58, L"Deck 58 - Condee Petie..." }, // [Mage Village] Black Mage 111 + Granin Miller (Condee Petie) + Tom's Mother (Alexandria 3+)
	{ 59, L"Deck 59 - Condee Petie..." }, // [Condee Petie] Barbara Gibgab + Ryan (Alexandria 3) + Man (Alexandria 3+)
	{ 60, L"Deck 60 - Alexandria" }, // [Alexandria 3] Synthesist + Shopkeeper (Alexandria 3+)
	{ 61, L"Deck 61 - Alexandria" }, // [Alexandria 3] Shopkeeper + Ryan (Alexandria 3+)
	{ 62, L"Deck 62 - Alexandria" }, // [Alexandria 3] Nikolai + Hippolady (Alexandria 3+)
	{ 63, L"Deck 63 - Alexandria..." }, // [Chocobo Paradise] Navi Chocobo + Hippaul (Alexandria 3 & 3+)
	{ 64, L"Deck 64 - Alexandria" }, // [Alexandria 3+] Fish Man
	{ 65, L"Deck 65 - Alexandria" }, // [Alexandria 3+] Red Mage
	{ 66, L"Deck 66 - Alexandria" }, // [Alexandria 3+] Red Mage
	{ 67, L"Deck 67 - Alexandria" }, // [Alexandria 3+] Ovenmeister
	{ 68, L"Deck 68 - Alexandria" }, // [Alexandria 3+] Nikolai
	{ 69, L"Deck 69 - Alexandria" }, // [Alexandria 3+] Michelle
	{ 70, L"Deck 70 - Alexandria" }, // [Alexandria 3+] Immigrant From Lindblum
	{ 71, L"Deck 71 - Alexandria" }, // [Alexandria 3+] Old Woman
	{ 72, L"Deck 72 - Alexandria" }, // [Alexandria 3+] Old Man
	{ 73, L"Deck 73 - Alexandria" }, // [Alexandria 3+] Ilia
	{ 74, L"Deck 74 - Lindblum" }, // [Lindblum 3] Craftsman
	{ 75, L"Deck 75 - Lindblum" }, // [Lindblum 3] Young Lady
	{ 76, L"Deck 76 - Lindblum" }, // [Lindblum 3] Olivier
	{ 77, L"Deck 77 - Lindblum" }, // [Lindblum 3] Joanna
	{ 78, L"Deck 78 - Lindblum" }, // [Lindblum 3] Carpenter's Apprentice 2
	{ 79, L"Deck 79 - Lindblum" }, // [Lindblum 3] Female Customer
	{ 80, L"Deck 80 - Lindblum" }, // [Lindblum 3] Industrial District Engineer
	{ 81, L"Deck 81 - Patient Fanni..." }, // [Treno] Patient Fanni + Card Boy (Lindblum 3)
	{ 82, L"Deck 82 - Daguerreo" }, // [Daguerreo] Adventurer
	{ 83, L"Deck 83 - Moody Lafrecia..." }, // [Treno] Moody Lafrecia + Card Freak (Lindblum 3) + Booklover (Daguerreo)
	{ 84, L"Deck 84 - Daguerreo..." }, // [Chocobo Paradise] Gold Chocobo + Bookworm (Daguerreo)
	{ 85, L"Deck 85 - Daguerreo" }, // [Daguerreo] Synthesist Expert
	{ 86, L"Deck 86 - Daguerreo" }, // [Daguerreo] Scholar
	{ 87, L"Deck 87 - Daguerreo" }, // [Daguerreo] Scholar
	{ 88, L"Deck 88 - Daguerreo" }, // [Daguerreo] Zebolt
	{ 89, L"Deck 89 - Daguerreo" }, // [Daguerreo] Librarian
	{ 90, L"Deck 90 - Daguerreo" }, // [Daguerreo] Old Man
	{ 91, L"Deck 91 - Daguerreo" }, // [Daguerreo] Weaponsmith
	{ 92, L"Deck 92 - Daguerreo" }, // [Daguerreo] Adventurer
	{ 93, L"Deck 93 - Mage Village" }, // [Mage Village 4] Black Mage 24
	{ 94, L"Deck 94 - Mage Village" }, // [Mage Village 4] Genome
	{ 95, L"Deck 95 - Mage Village" }, // [Mage Village 4] Black Mage 123
	{ 96, L"Deck 96 - Mage Village" }, // [Mage Village 4] Black Mage 234
	{ 97, L"Deck 97 - Mage Village" }, // [Mage Village 4] Genome
	{ 98, L"Deck 98 - Mage Village" }, // [Mage Village 4] Black Mage 189
	{ 99, L"Deck 99 - Mage Village" }, // [Mage Village 4] Black Mage 44
	{ 100, L"Deck 100 - Mage Village..." }, // [Madain Sari] Lani + Genome (Mage Village 4)
	{ 101, L"Deck 101 - Mage Village" }, // [Mage Village 4] Black Mage 192
	{ 102, L"Deck 102 - Mage Village" }, // [Mage Village 4] Black Mage 56
	{ 103, L"Deck 103 - Mage Village" }, // [Mage Village 4] Black Mage 33
	{ 104, L"Deck 104 - Lucious Ellie..." }, // + Genome (Mage Village 4)
	{ 105, L"Deck 105 - Card Freak Cil" },
	{ 106, L"Deck 106 - Gambler Grecchio" },
	{ 107, L"Deck 107 - Straight Shooter Shak" },
	{ 108, L"Deck 108 - Fat Chocobo" },
	{ 109, L"Deck 109 - Bomb Master Shannon" },
	{ 110, L"Deck 110 - Malboro Master Joe" },
	{ 111, L"Deck 111 - Worker #9" },
	{ 112, L"Deck 112 - Gilbert's Teacher" },
	{ 113, L"Deck 113 - Eidolon Master Leyra" },
	{ 114, L"Deck 114 - Beast Master Gilbert" },
	{ 115, L"Deck 115 - Magic Master Thalisa" },
	{ 116, L"Deck 116 - Daguerreo" }, // Gilgamesh
	{ 117, L"Deck 117 - Weapon Master Hunt" },
	{ 118, L"Deck 118 - Master Phantom" },
	{ 119, L"Deck 119 - Defense Phantom" },
	{ 120, L"Deck 120 - Strong Phantom" },
	{ 121, L"Deck 121 - Rare Phantom" },
	{ 122, L"Deck 122 - Dark Phantom" },
	{ 123, L"Deck 123 - Oglop Master Bon" },
	{ 124, L"Deck 124 - Attic Man Wake" },
	{ 125, L"Deck 125 - Mario of the Slums" },
	{ 126, L"Deck 126 - Cardona Bishop" },
	{ 127, L"Deck 127 - Sailor Erin" },
	{ 128, L"Deck 128" },
	{ 129, L"Deck 129" },
	{ 130, L"Deck 130" },
	{ 131, L"Deck 131" },
	{ 132, L"Deck 132" },
	{ 133, L"Deck 133" },
	{ 134, L"Deck 134" },
	{ 135, L"Deck 135" },
	{ 136, L"Deck 136" },
	{ 137, L"Deck 137" },
	{ 138, L"Deck 138" },
	{ 139, L"Deck 139" },
	{ 140, L"Deck 140" },
	{ 141, L"Deck 141" },
	{ 142, L"Deck 142" },
	{ 143, L"Deck 143" },
	{ 144, L"Deck 144" },
	{ 145, L"Deck 145" },
	{ 146, L"Deck 146" },
	{ 147, L"Deck 147" },
	{ 148, L"Deck 148" },
	{ 149, L"Deck 149" },
	{ 150, L"Deck 150" },
	{ 151, L"Deck 151" },
	{ 152, L"Deck 152" },
	{ 153, L"Deck 153" },
	{ 154, L"Deck 154" },
	{ 155, L"Deck 155" },
	{ 156, L"Deck 156" },
	{ 157, L"Deck 157" },
	{ 158, L"Deck 158" },
	{ 159, L"Deck 159" },
	{ 160, L"Deck 160" },
	{ 161, L"Deck 161" },
	{ 162, L"Deck 162" },
	{ 163, L"Deck 163" },
	{ 164, L"Deck 164" },
	{ 165, L"Deck 165" },
	{ 166, L"Deck 166" },
	{ 167, L"Deck 167" },
	{ 168, L"Deck 168" },
	{ 169, L"Deck 169" },
	{ 170, L"Deck 170" },
	{ 171, L"Deck 171" },
	{ 172, L"Deck 172" },
	{ 173, L"Deck 173" },
	{ 174, L"Deck 174" },
	{ 175, L"Deck 175" },
	{ 176, L"Deck 176" },
	{ 177, L"Deck 177" },
	{ 178, L"Deck 178" },
	{ 179, L"Deck 179" },
	{ 180, L"Deck 180" },
	{ 181, L"Deck 181" },
	{ 182, L"Deck 182" },
	{ 183, L"Deck 183" },
	{ 184, L"Deck 184" },
	{ 185, L"Deck 185" },
	{ 186, L"Deck 186" },
	{ 187, L"Deck 187" },
	{ 188, L"Deck 188" },
	{ 189, L"Deck 189" },
	{ 190, L"Deck 190" },
	{ 191, L"Deck 191" },
	{ 192, L"Deck 192" },
	{ 193, L"Deck 193" },
	{ 194, L"Deck 194" },
	{ 195, L"Deck 195" },
	{ 196, L"Deck 196" },
	{ 197, L"Deck 197" },
	{ 198, L"Deck 198" },
	{ 199, L"Deck 199" },
	{ 200, L"Deck 200" },
	{ 201, L"Deck 201" },
	{ 202, L"Deck 202" },
	{ 203, L"Deck 203" },
	{ 204, L"Deck 204" },
	{ 205, L"Deck 205" },
	{ 206, L"Deck 206" },
	{ 207, L"Deck 207" },
	{ 208, L"Deck 208" },
	{ 209, L"Deck 209" },
	{ 210, L"Deck 210" },
	{ 211, L"Deck 211" },
	{ 212, L"Deck 212" },
	{ 213, L"Deck 213" },
	{ 214, L"Deck 214" },
	{ 215, L"Deck 215" },
	{ 216, L"Deck 216" },
	{ 217, L"Deck 217" },
	{ 218, L"Deck 218" },
	{ 219, L"Deck 219" },
	{ 220, L"Deck 220" },
	{ 221, L"Deck 221" },
	{ 222, L"Deck 222" },
	{ 223, L"Deck 223" },
	{ 224, L"Deck 224" },
	{ 225, L"Deck 225" },
	{ 226, L"Deck 226" },
	{ 227, L"Deck 227" },
	{ 228, L"Deck 228" },
	{ 229, L"Deck 229" },
	{ 230, L"Deck 230" },
	{ 231, L"Deck 231" },
	{ 232, L"Deck 232" },
	{ 233, L"Deck 233" },
	{ 234, L"Deck 234" },
	{ 235, L"Deck 235" },
	{ 236, L"Deck 236" },
	{ 237, L"Deck 237" },
	{ 238, L"Deck 238" },
	{ 239, L"Deck 239" },
	{ 240, L"Deck 240" },
	{ 241, L"Deck 241" },
	{ 242, L"Deck 242" },
	{ 243, L"Deck 243" },
	{ 244, L"Deck 244" },
	{ 245, L"Deck 245" },
	{ 246, L"Deck 246" },
	{ 247, L"Deck 247" },
	{ 248, L"Deck 248" },
	{ 249, L"Deck 249" },
	{ 250, L"Deck 250" },
	{ 251, L"Deck 251" },
	{ 252, L"Deck 252" },
	{ 253, L"Deck 253" },
	{ 254, L"Deck 254" },
	{ 255, L"Deck 255" }
};

static SortedChoiceItem HADES_STRING_CARD_NAME[] = {
	{ 0, L"Goblin" },
	{ 1, L"Fang" },
	{ 2, L"Skeleton" },
	{ 3, L"Flan" },
	{ 4, L"Zaghnol" },
	{ 5, L"Lizard Man" },
	{ 6, L"Zombie" },
	{ 7, L"Bomb" },
	{ 8, L"Ironite" },
	{ 9, L"Sahagin" },
	{ 10, L"Yeti" },
	{ 11, L"Mimic" },
	{ 12, L"Wyerd" },
	{ 13, L"Mandragora" },
	{ 14, L"Crawler" },
	{ 15, L"Sand Scorpion" },
	{ 16, L"Nymph" },
	{ 17, L"Sand Golem" },
	{ 18, L"Zuu" },
	{ 19, L"Dragonfly" },
	{ 20, L"Carrion Worm" },
	{ 21, L"Cerberus" },
	{ 22, L"Antlion" },
	{ 23, L"Cactuar" },
	{ 24, L"Gimme Cat" },
	{ 25, L"Ragtime Mouse" },
	{ 26, L"Hedgehog Pie" },
	{ 27, L"Ralvuimago" },
	{ 28, L"Ochu" },
	{ 29, L"Troll" },
	{ 30, L"Blazer Beetle" },
	{ 31, L"Abomination" },
	{ 32, L"Zemzelett" },
	{ 33, L"Stroper" },
	{ 34, L"Tantarian" },
	{ 35, L"Grand Dragon" },
	{ 36, L"Feather Circle" },
	{ 37, L"Hecteyes" },
	{ 38, L"Ogre" },
	{ 39, L"Armstrong" },
	{ 40, L"Ash" },
	{ 41, L"Wraith" },
	{ 42, L"Gargoyle" },
	{ 43, L"Vepal" },
	{ 44, L"Grimlock" },
	{ 45, L"Tonberry" },
	{ 46, L"Veteran" },
	{ 47, L"Garuda" },
	{ 48, L"Malboro" },
	{ 49, L"Mover" },
	{ 50, L"Abadon" },
	{ 51, L"Behemoth" },
	{ 52, L"Iron Man" },
	{ 53, L"Nova Dragon" },
	{ 54, L"Ozma" },
	{ 55, L"Hades" },
	{ 56, L"Holy" },
	{ 57, L"Meteor" },
	{ 58, L"Flare" },
	{ 59, L"Shiva" },
	{ 60, L"Ifrit" },
	{ 61, L"Ramuh" },
	{ 62, L"Atomos" },
	{ 63, L"Odin" },
	{ 64, L"Leviathan" },
	{ 65, L"Bahamut" },
	{ 66, L"Ark" },
	{ 67, L"Fenrir" },
	{ 68, L"Madeen" },
	{ 69, L"Alexander" },
	{ 70, L"Excalibur II" },
	{ 71, L"Ultima Weapon" },
	{ 72, L"Masamune" },
	{ 73, L"Elixir" },
	{ 74, L"Dark Matter" },
	{ 75, L"Ribbon" },
	{ 76, L"Tiger Racket" },
	{ 77, L"Save the Queen" },
	{ 78, L"Genji" },
	{ 79, L"Mythril Sword" },
	{ 80, L"Blue Narciss" },
	{ 81, L"Hilda Garde III" },
	{ 82, L"Invincible" },
	{ 83, L"Cargo Ship" },
	{ 84, L"Hilda Garde I" },
	{ 85, L"Red Rose" },
	{ 86, L"Theater Ship" },
	{ 87, L"Viltgance" },
	{ 88, L"Chocobo" },
	{ 89, L"Fat Chocobo" },
	{ 90, L"Mog" },
	{ 91, L"Frog" },
	{ 92, L"Oglop" },
	{ 93, L"Alexandria" },
	{ 94, L"Lindblum" },
	{ 95, L"Twin Moons" },
	{ 96, L"Gargant" },
	{ 97, L"Namingway" },
	{ 98, L"Boco" },
	{ 99, L"Airship" }
};

#endif
