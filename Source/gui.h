///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 24 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class PreviewTextCtrl;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/aui/auibook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/bmpbuttn.h>
#include <wx/notebook.h>
#include <wx/statbmp.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/statbox.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/checklst.h>
#include <wx/radiobox.h>
#include <wx/scrolbar.h>
#include <wx/statline.h>
#include <wx/treelist.h>
#include <wx/radiobut.h>
#include <wx/gauge.h>
#include <wx/valtext.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_TEXT 1000
#define wxID_IMPTEXT 1001
#define wxID_UITEXT 1002
#define wxID_IMPUITEXT 1003
#define wxID_ENMYSCRIPT 1004
#define wxID_IMPENMYSCRIPT 1005
#define wxID_WORLDSCRIPT 1006
#define wxID_IMPWORLDSCRIPT 1007
#define wxID_FIELDSCRIPT 1008
#define wxID_IMPFIELDSCRIPT 1009
#define wxID_BACKGROUND 1010
#define wxID_TOOLMOD 1011
#define wxID_BACKEDIT 1012
#define wxID_ASSETS 1013
#define wxID_NAME 1014
#define wxID_PERFNAME 1015
#define wxID_EFFECT 1016
#define wxID_POWER 1017
#define wxID_SE1 1018
#define wxID_SE2 1019
#define wxID_SE3 1020
#define wxID_SE4 1021
#define wxID_SE5 1022
#define wxID_SE6 1023
#define wxID_SE7 1024
#define wxID_SE8 1025
#define wxID_STATUS 1026
#define wxID_ACCURACY 1027
#define wxID_MP 1028
#define wxID_CAN_USE_IN_MENU 1029
#define wxID_HIDE_AP_PROGRESSION 1030
#define wxID_MP_COST_BOOST 1031
#define wxID_TARGETTYPE 1032
#define wxID_TARGETAMOUNT 1033
#define wxID_TARGETPRIORITY 1034
#define wxID_CAN_TARGET_DEAD 1035
#define wxID_TARGET_DEAD_FIRST 1036
#define wxID_TARGET_CAMERA 1037
#define wxID_TARGETPANEL 1038
#define wxID_SF1 1039
#define wxID_SF2 1040
#define wxID_SF3 1041
#define wxID_SF4 1042
#define wxID_SF5 1043
#define wxID_SF6 1044
#define wxID_SF7 1045
#define wxID_SF8 1046
#define wxID_MAGICSTONE 1047
#define wxID_PANEL 1048
#define wxID_LINK 1049
#define wxID_OBJECTFIRST 1050
#define wxID_OBJECTLAST 1051
#define wxID_SPEED 1052
#define wxID_STRENGTH 1053
#define wxID_MAGIC 1054
#define wxID_SPIRIT 1055
#define wxID_ABILITYSET 1056
#define wxID_ABILITY 1057
#define wxID_ABILITYCOST 1058
#define wxID_COMMAND1 1059
#define wxID_COMMAND2 1060
#define wxID_TRANCE_COMMAND1 1061
#define wxID_TRANCE_COMMAND2 1062
#define wxID_TRANCE_ATTACK 1063
#define wxID_EQUIPSET 1064
#define wxID_WEAPON 1065
#define wxID_HEAD 1066
#define wxID_WRIST 1067
#define wxID_ARMOR 1068
#define wxID_ACCESSORY 1069
#define wxID_EXP 1070
#define wxID_HP 1071
#define wxID_MAGICSWD 1072
#define wxID_BATTLEHELP 1073
#define wxID_IT1 1074
#define wxID_IT2 1075
#define wxID_IT3 1076
#define wxID_IT4 1077
#define wxID_IT5 1078
#define wxID_IT6 1079
#define wxID_IT7 1080
#define wxID_IT8 1081
#define wxID_PRICE 1082
#define wxID_IA16 1083
#define wxID_IA15 1084
#define wxID_IA14 1085
#define wxID_IA13 1086
#define wxID_IA12 1087
#define wxID_IA11 1088
#define wxID_IA10 1089
#define wxID_IA9 1090
#define wxID_IA8 1091
#define wxID_IA7 1092
#define wxID_IA6 1093
#define wxID_IA5 1094
#define wxID_IA4 1095
#define wxID_IA3 1096
#define wxID_IA2 1097
#define wxID_IA1 1098
#define wxID_STAT 1099
#define wxID_SKILL1 1100
#define wxID_SKILL2 1101
#define wxID_SKILL3 1102
#define wxID_MENUPOSUP 1103
#define wxID_MENUPOSDOWN 1104
#define wxID_EQUIPPOSUP 1105
#define wxID_EQUIPPOSDOWN 1106
#define wxID_ICON 1107
#define wxID_ICONCOLOR 1108
#define wxID_IS1 1109
#define wxID_IS2 1110
#define wxID_IS3 1111
#define wxID_IS4 1112
#define wxID_IS5 1113
#define wxID_IS6 1114
#define wxID_IS7 1115
#define wxID_IS8 1116
#define wxID_IS9 1117
#define wxID_IS10 1118
#define wxID_IS11 1119
#define wxID_IS12 1120
#define wxID_IS13 1121
#define wxID_IS14 1122
#define wxID_IS15 1123
#define wxID_IS16 1124
#define wxID_IS17 1125
#define wxID_IS18 1126
#define wxID_IS19 1127
#define wxID_IS20 1128
#define wxID_IS21 1129
#define wxID_IS22 1130
#define wxID_IS23 1131
#define wxID_IS24 1132
#define wxID_IS25 1133
#define wxID_IS26 1134
#define wxID_IS27 1135
#define wxID_IS28 1136
#define wxID_IS29 1137
#define wxID_IS30 1138
#define wxID_IS31 1139
#define wxID_IS32 1140
#define wxID_WEAPONFORMULA 1141
#define wxID_WEAPONPOWER 1142
#define wxID_WE1 1143
#define wxID_WE2 1144
#define wxID_WE3 1145
#define wxID_WE4 1146
#define wxID_WE5 1147
#define wxID_WE6 1148
#define wxID_WE7 1149
#define wxID_WE8 1150
#define wxID_WEAPONSTATUS 1151
#define wxID_WEAPONACC 1152
#define wxID_WF1 1153
#define wxID_WF2 1154
#define wxID_WF3 1155
#define wxID_WF4 1156
#define wxID_WF5 1157
#define wxID_WF6 1158
#define wxID_WF7 1159
#define wxID_WF8 1160
#define wxID_WEAPONUNK1 1161
#define wxID_WEAPONUNK2 1162
#define wxID_WEAPONMODEL 1163
#define wxID_DEFENCE 1164
#define wxID_EVADE 1165
#define wxID_MDEFENCE 1166
#define wxID_MEVADE 1167
#define wxID_KEYNAME 1168
#define wxID_KEYHELP 1169
#define wxID_KEYDESCRIPTION 1170
#define wxID_SYNTH 1171
#define wxID_RECIPE1 1172
#define wxID_RECIPE2 1173
#define wxID_SS1 1174
#define wxID_SS2 1175
#define wxID_SS3 1176
#define wxID_SS4 1177
#define wxID_SS5 1178
#define wxID_SS6 1179
#define wxID_SS7 1180
#define wxID_SS8 1181
#define wxID_SCRIPT 1182
#define wxID_ENTRY 1183
#define wxID_SCENE 1184
#define wxID_BF1 1185
#define wxID_BF2 1186
#define wxID_BF3 1187
#define wxID_BF4 1188
#define wxID_BF5 1189
#define wxID_BF6 1190
#define wxID_BF7 1191
#define wxID_BF8 1192
#define wxID_BF9 1193
#define wxID_BF10 1194
#define wxID_BF11 1195
#define wxID_BF12 1196
#define wxID_BF13 1197
#define wxID_BF14 1198
#define wxID_BF15 1199
#define wxID_BF16 1200
#define wxID_STATNAME 1201
#define wxID_LVL 1202
#define wxID_ATTACK 1203
#define wxID_STEAL1 1204
#define wxID_STEAL2 1205
#define wxID_STEAL3 1206
#define wxID_STEAL4 1207
#define wxID_DROP1 1208
#define wxID_DROP2 1209
#define wxID_DROP3 1210
#define wxID_DROP4 1211
#define wxID_CARD 1212
#define wxID_GILS 1213
#define wxID_DEFATTACK 1214
#define wxID_MODEL 1215
#define wxID_EA1 1216
#define wxID_EA2 1217
#define wxID_EA3 1218
#define wxID_EA4 1219
#define wxID_EA5 1220
#define wxID_EA6 1221
#define wxID_EA7 1222
#define wxID_EA8 1223
#define wxID_EI1 1224
#define wxID_EI2 1225
#define wxID_EI3 1226
#define wxID_EI4 1227
#define wxID_EI5 1228
#define wxID_EI6 1229
#define wxID_EI7 1230
#define wxID_EI8 1231
#define wxID_EH1 1232
#define wxID_EH2 1233
#define wxID_EH3 1234
#define wxID_EH4 1235
#define wxID_EH5 1236
#define wxID_EH6 1237
#define wxID_EH7 1238
#define wxID_EH8 1239
#define wxID_EW1 1240
#define wxID_EW2 1241
#define wxID_EW3 1242
#define wxID_EW4 1243
#define wxID_EW5 1244
#define wxID_EW6 1245
#define wxID_EW7 1246
#define wxID_EW8 1247
#define wxID_EC1 1248
#define wxID_EC2 1249
#define wxID_EC3 1250
#define wxID_EC4 1251
#define wxID_EC5 1252
#define wxID_EC6 1253
#define wxID_EC7 1254
#define wxID_EC8 1255
#define wxID_SI1 1256
#define wxID_SI2 1257
#define wxID_SI3 1258
#define wxID_SI4 1259
#define wxID_SI5 1260
#define wxID_SI6 1261
#define wxID_SI7 1262
#define wxID_SI8 1263
#define wxID_SI9 1264
#define wxID_SI10 1265
#define wxID_SI11 1266
#define wxID_SI12 1267
#define wxID_SI13 1268
#define wxID_SI14 1269
#define wxID_SI15 1270
#define wxID_SI16 1271
#define wxID_SI17 1272
#define wxID_SI18 1273
#define wxID_SI19 1274
#define wxID_SI20 1275
#define wxID_SI21 1276
#define wxID_SI22 1277
#define wxID_SI23 1278
#define wxID_SI24 1279
#define wxID_SI25 1280
#define wxID_SI26 1281
#define wxID_SI27 1282
#define wxID_SI28 1283
#define wxID_SI29 1284
#define wxID_SI30 1285
#define wxID_SI31 1286
#define wxID_SI32 1287
#define wxID_SA1 1288
#define wxID_SA2 1289
#define wxID_SA3 1290
#define wxID_SA4 1291
#define wxID_SA5 1292
#define wxID_SA6 1293
#define wxID_SA7 1294
#define wxID_SA8 1295
#define wxID_SA9 1296
#define wxID_SA10 1297
#define wxID_SA11 1298
#define wxID_SA12 1299
#define wxID_SA13 1300
#define wxID_SA14 1301
#define wxID_SA15 1302
#define wxID_SA16 1303
#define wxID_SA17 1304
#define wxID_SA18 1305
#define wxID_SA19 1306
#define wxID_SA20 1307
#define wxID_SA21 1308
#define wxID_SA22 1309
#define wxID_SA23 1310
#define wxID_SA24 1311
#define wxID_SA25 1312
#define wxID_SA26 1313
#define wxID_SA27 1314
#define wxID_SA28 1315
#define wxID_SA29 1316
#define wxID_SA30 1317
#define wxID_SA31 1318
#define wxID_SA32 1319
#define wxID_ST1 1320
#define wxID_ST2 1321
#define wxID_ST3 1322
#define wxID_ST4 1323
#define wxID_ST5 1324
#define wxID_ST6 1325
#define wxID_ST7 1326
#define wxID_ST8 1327
#define wxID_ST9 1328
#define wxID_ST10 1329
#define wxID_ST11 1330
#define wxID_ST12 1331
#define wxID_ST13 1332
#define wxID_ST14 1333
#define wxID_ST15 1334
#define wxID_ST16 1335
#define wxID_ST17 1336
#define wxID_ST18 1337
#define wxID_ST19 1338
#define wxID_ST20 1339
#define wxID_ST21 1340
#define wxID_ST22 1341
#define wxID_ST23 1342
#define wxID_ST24 1343
#define wxID_ST25 1344
#define wxID_ST26 1345
#define wxID_ST27 1346
#define wxID_ST28 1347
#define wxID_ST29 1348
#define wxID_ST30 1349
#define wxID_ST31 1350
#define wxID_ST32 1351
#define wxID_BLUEMAGIC 1352
#define wxID_SPELLNAME 1353
#define wxID_SPELLMP 1354
#define wxID_ANIM 1355
#define wxID_SEQANIM 1356
#define wxID_FREQUENCE 1357
#define wxID_AMOUNT 1358
#define wxID_AP 1359
#define wxID_CAMERA 1360
#define wxID_ENEMY1 1361
#define wxID_TARGETABLE1 1362
#define wxID_LINK1 1363
#define wxID_POSX1 1364
#define wxID_POSY1 1365
#define wxID_POSZ1 1366
#define wxID_ANGLE1 1367
#define wxID_ENEMY2 1368
#define wxID_TARGETABLE2 1369
#define wxID_LINK2 1370
#define wxID_POSX2 1371
#define wxID_POSY2 1372
#define wxID_POSZ2 1373
#define wxID_ANGLE2 1374
#define wxID_ENEMY3 1375
#define wxID_TARGETABLE3 1376
#define wxID_LINK3 1377
#define wxID_POSX3 1378
#define wxID_POSY3 1379
#define wxID_POSZ3 1380
#define wxID_ANGLE3 1381
#define wxID_ENEMY4 1382
#define wxID_TARGETABLE4 1383
#define wxID_LINK4 1384
#define wxID_POSX4 1385
#define wxID_POSY4 1386
#define wxID_POSZ4 1387
#define wxID_ANGLE4 1388
#define wxID_POINTS 1389
#define wxID_TYPEPLY 1390
#define wxID_ATTPLY 1391
#define wxID_DEFPLY 1392
#define wxID_MDEFPLY 1393
#define wxID_TYPENPC 1394
#define wxID_ATTNPC 1395
#define wxID_DEFNPC 1396
#define wxID_MDEFNPC 1397
#define wxID_DIFFICULTY 1398
#define wxID_DECKSET 1399
#define wxID_PRELOAD 1400
#define wxID_WORLD 1401
#define wxID_BATTLE1 1402
#define wxID_BATTLE2 1403
#define wxID_BATTLE3 1404
#define wxID_BATTLE4 1405
#define wxID_TEXTURE 1406
#define wxID_SEQUENCE 1407
#define wxID_DISCARD 1408
#define wxID_EB1 1409
#define wxID_EB2 1410
#define wxID_EB3 1411
#define wxID_EB4 1412
#define wxID_EB5 1413
#define wxID_EB6 1414
#define wxID_EB7 1415
#define wxID_EB8 1416
#define wxID_WINCOLOR 1417
#define wxID_TEXTPREVIEW 1418
#define wxID_STEAMLANG 1419
#define wxID_RESOLUTION 1420
#define wxID_ALPHABET 1421
#define wxID_SPELL 1422
#define wxID_SUPPORT 1423
#define wxID_CMD 1424
#define wxID_PARTY_SPECIAL 1425
#define wxID_ITEM 1426
#define wxID_SHOP 1427
#define wxID_ENMY 1428
#define wxID_FIELD 1429
#define wxID_SPELL_ANIM 1430
#define wxID_MENU_UI 1431
#define wxID_ASSEMBLY 1432
#define wxID_SET 1433
#define wxID_BUBBLE 1434
#define wxID_OPCODE 1435
#define wxID_DEFAULTARG1 1436
#define wxID_DEFAULTARG2 1437
#define wxID_DEFAULTARG3 1438
#define wxID_TOKENIZE 1439
#define wxID_48 1440
#define wxID_PALETTE 1441
#define wxID_PALSEL 1442
#define wxID_MODIFY 1443
#define wxID_IMPORT 1444
#define wxID_POSX 1445
#define wxID_POSY 1446
#define wxID_SIZEX 1447
#define wxID_SIZEY 1448
#define wxID_TILE 1449
#define wxID_EXPORT 1450
#define wxID_INT 1451
#define wxID_LONG 1452
#define wxID_HEXA 1453
#define wxID_BUTTON 1454
#define wxID_LIST 1455
#define wxID_SPLIST 1456
#define wxID_STATUSA 1457
#define wxID_STATUSB 1458
#define wxID_FBACK 1459
#define wxID_FWALK 1460
#define wxID_PARSE 1461
#define wxID_FUNCTION 1462
#define wxID_VARIABLE 1463
#define wxID_VARCODE 1464
#define wxID_TYPE 1465
#define wxID_FULLLIST 1466
#define wxID_MAPLIST 1467
#define wxID_SORTLAYER 1468
#define wxID_REVERTLAYER 1469
#define wxID_FORMAT 1470
#define wxID_MASSFORMAT 1471
#define wxID_REOPEN 1472
#define wxID_PDATA11 1473
#define wxID_PDATA12 1474
#define wxID_PDATA13 1475
#define wxID_PDATA14 1476
#define wxID_PDATA15 1477
#define wxID_PDATA16 1478
#define wxID_PDATA17 1479
#define wxID_PDATA18 1480
#define wxID_PDATA19 1481
#define wxID_PDATA2 1482
#define wxID_PDATA3 1483
#define wxID_PDATA4 1484
#define wxID_PDATA5 1485
#define wxID_PDATA61 1486
#define wxID_PDATA62 1487
#define wxID_PDATA63 1488
#define wxID_PDATA7 1489
#define wxID_LEVEL0 1490
#define wxID_LEVEL1 1491
#define wxID_LEVEL2 1492
#define wxID_LEVEL3 1493
#define wxID_LEVEL4 1494
#define wxID_LEVEL5 1495
#define wxID_LEVEL6 1496
#define wxID_LEVEL7 1497
#define wxID_LEVEL8 1498
#define wxID_LEVEL9 1499
#define wxID_LEVEL10 1500
#define wxID_LEVEL11 1501
#define wxID_LEVEL12 1502
#define wxID_LEVEL13 1503
#define wxID_LEVEL14 1504
#define wxID_LEVEL15 1505
#define wxID_LEVEL16 1506
#define wxID_LEVEL17 1507
#define wxID_LEVEL18 1508
#define wxID_LEVEL19 1509
#define wxID_LEVEL20 1510
#define wxID_LEVEL21 1511
#define wxID_LEVEL22 1512
#define wxID_LEVEL23 1513
#define wxID_LEVEL24 1514
#define wxID_LEVEL25 1515
#define wxID_LEVEL26 1516
#define wxID_LEVEL27 1517
#define wxID_RESOURCES 1518
#define wxID_MAINDATA 1519
#define wxID_SHARED0 1520
#define wxID_SHARED1 1521
#define wxID_SHARED2 1522
#define wxID_SHARED3 1523
#define wxID_SHARED4 1524
#define wxID_SHARED5 1525
#define wxID_SHARED6 1526
#define wxID_SHARED7 1527
#define wxID_SHARED8 1528
#define wxID_SHARED9 1529
#define wxID_SHARED10 1530
#define wxID_SHARED11 1531
#define wxID_SHARED12 1532
#define wxID_SHARED13 1533
#define wxID_SHARED14 1534
#define wxID_SHARED15 1535
#define wxID_SHARED16 1536
#define wxID_SHARED17 1537
#define wxID_SHARED18 1538
#define wxID_SHARED19 1539
#define wxID_SHARED20 1540
#define wxID_SHARED21 1541
#define wxID_SHARED22 1542
#define wxID_SHARED23 1543
#define wxID_SHARED24 1544
#define wxID_SHARED25 1545
#define wxID_SHARED26 1546
#define wxID_SHARED27 1547
#define wxID_SHARED28 1548
#define wxID_PATH 1549
#define wxID_FOLDER64 1550
#define wxID_FOLDER86 1551

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxMenu* m_menuOption;
		wxMenu* m_menuBatch;
		wxMenu* m_menuTools;
		wxMenu* m_menuHelp;
		wxPanel* m_background;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnFramePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnOpenClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenHWSClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCloseClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCloseAllClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveHWSClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveSteamClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportPPFClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveBinClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPreferencesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortSpellClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortSupportClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortCommandClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortItemClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortKeyItemClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortEnemyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortCardClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortTextClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortWorldClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortFieldClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortBattleSceneClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortSpellAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBatchExportClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBatchImportClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPanelChanged( wxAuiNotebookEvent& event ) { event.Skip(); }
		
	
	public:
		wxMenuItem* m_open;
		wxMenuItem* m_openhws;
		wxMenuItem* m_close;
		wxMenuItem* m_closeall;
		wxMenuItem* m_savehws;
		wxMenuItem* m_savesteam;
		wxMenuItem* m_exportppf;
		wxMenuItem* m_savebin;
		wxMenuItem* m_preferences;
		wxMenuItem* m_exit;
		wxMenuItem* m_sortspell;
		wxMenuItem* m_sortsupport;
		wxMenuItem* m_sortcmd;
		wxMenuItem* m_sortitem;
		wxMenuItem* m_sortkeyitem;
		wxMenuItem* m_sortenemy;
		wxMenuItem* m_enemyshowid;
		wxMenuItem* m_editsimilarenemy;
		wxMenuItem* m_sortcard;
		wxMenuItem* m_sorttext;
		wxMenuItem* m_sortworld;
		wxMenuItem* m_sortfield;
		wxMenuItem* m_sortbattlescene;
		wxMenuItem* m_sortspellanim;
		wxMenuItem* m_exporttext;
		wxMenuItem* m_importtext;
		wxMenuItem* m_exportuitext;
		wxMenuItem* m_importuitext;
		wxMenuItem* m_exportenemyscript;
		wxMenuItem* m_importenemyscript;
		wxMenuItem* m_exportworldscript;
		wxMenuItem* m_importworldscript;
		wxMenuItem* m_exportfieldscript;
		wxMenuItem* m_importfieldscript;
		wxMenuItem* m_exportfieldbackground;
		wxMenuItem* m_modmanager;
		wxMenuItem* m_backgroundeditor;
		wxMenuItem* m_unityviewer;
		wxAuiNotebook* m_cdbook;
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Hades Workshop"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,500 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CDPanel
///////////////////////////////////////////////////////////////////////////////
class CDPanel : public wxPanel 
{
	private:
	
	protected:
		wxNotebook* m_notebookmain;
		wxPanel* m_panelparty;
		wxNotebook* m_notebookparty;
		wxPanel* m_panel21;
		wxScrolledWindow* m_spellscrolledwindow;
		wxStaticText* m_spellnamelabel;
		wxButton* m_spellnamebutton;
		wxStaticText* m_staticText265;
		wxChoice* m_spellperformname;
		wxStaticText* m_spellhelplabel;
		wxButton* m_spellhelpbutton;
		wxStaticText* m_spelleffectlabel;
		wxScrolledWindow* m_spelleffecthelpwindow;
		wxStaticText* m_spelleffecthelp;
		wxStaticText* m_spellpowerlabel;
		wxStaticText* m_spellelementlabel;
		wxCheckBox* m_spellelement1;
		wxCheckBox* m_spellelement2;
		wxCheckBox* m_spellelement3;
		wxCheckBox* m_spellelement4;
		wxCheckBox* m_spellelement5;
		wxCheckBox* m_spellelement6;
		wxCheckBox* m_spellelement7;
		wxCheckBox* m_spellelement8;
		wxStaticText* m_spellstatuslabel;
		wxChoice* m_spellstatus;
		wxButton* m_spellstatusset;
		wxStaticText* m_spellaccuracylabel;
		wxSpinCtrl* m_spellaccuracy;
		wxStaticText* m_spellmplabel;
		wxSpinCtrl* m_spellmp;
		wxStaticText* m_spellmenuflag;
		wxCheckBox* m_spellmenuflagmenuuse;
		wxCheckBox* m_spellmenuflaghideap;
		wxCheckBox* m_spellmenuflagmpboost;
		wxStaticText* m_spellmodellabel;
		wxStaticText* m_spellmodel;
		wxButton* m_spellmodeledit;
		wxStaticText* m_spellmodelaltlabel;
		wxStaticText* m_spellmodelalt;
		wxButton* m_spellmodelaltedit;
		wxStaticText* m_spelltargettypelabel;
		wxChoice* m_spelltargettype;
		wxStaticText* m_spelltargetamountlabel;
		wxChoice* m_spelltargetamount;
		wxStaticText* m_spelltargetprioritylabel;
		wxChoice* m_spelltargetpriority;
		wxStaticText* m_spelltargetflaglabel;
		wxCheckBox* m_spelltargetflagdead;
		wxCheckBox* m_spelltargetflagdeadfirst;
		wxCheckBox* m_spelltargetflagcamera;
		wxStaticText* m_spelltargetpanellabel;
		wxChoice* m_spelltargetpanel;
		wxStaticText* m_spellflaglabel;
		wxCheckBox* m_spellflag1;
		wxCheckBox* m_spellflag2;
		wxCheckBox* m_spellflag3;
		wxCheckBox* m_spellflag4;
		wxCheckBox* m_spellflag5;
		wxCheckBox* m_spellflag6;
		wxCheckBox* m_spellflag7;
		wxCheckBox* m_spellflag8;
		wxPanel* m_panel24;
		wxScrolledWindow* m_supportscrolledwindow;
		wxStaticText* m_staticText113;
		wxButton* m_supportnamebutton;
		wxStaticText* m_staticText114;
		wxButton* m_supporthelpbutton;
		wxStaticText* m_staticText115;
		wxSpinCtrl* m_supportcost;
		wxPanel* m_panel22;
		wxScrolledWindow* m_cmdscrolledwindow;
		wxStaticText* m_cmdnamelabl;
		wxButton* m_cmdnamebutton;
		wxStaticText* m_cmdhelplabel;
		wxButton* m_cmdhelpbutton;
		wxStaticText* m_cmdpanellabel;
		wxChoice* m_cmdpanel;
		wxStaticText* m_cmdlinklabel;
		wxChoice* m_cmdlink;
		wxStaticText* m_cmdspelllabel;
		wxPanel* m_cmdspelllistpanel;
		wxButton* m_cmdaddspell;
		wxButton* m_cmdremovespell;
		wxStaticText* m_staticText27;
		wxListBox* m_cmdspellfulllist;
		wxListBox* m_cmdspelllist;
		wxBitmapButton* m_cmdmovespellup;
		wxBitmapButton* m_cmdmovespelldown;
		wxStaticText* m_cmdobjectslabel;
		wxPanel* m_cmdobjectpanel;
		wxStaticText* m_cmdobjectfirstlabel;
		wxStaticText* m_cmdobjectendlabel;
		wxChoice* m_cmdobjectfirst;
		wxChoice* m_cmdobjectlast;
		wxPanel* m_panel25;
		wxScrolledWindow* m_statcharscrolledwindow;
		wxStaticText* m_staticText1131;
		wxButton* m_statchardefaultnamebutton;
		wxStaticText* m_staticText11311;
		wxSpinCtrl* m_statcharspeed;
		wxStaticText* m_staticText113111;
		wxSpinCtrl* m_statcharstrength;
		wxStaticText* m_staticText113112;
		wxSpinCtrl* m_statcharmagic;
		wxStaticText* m_staticText113113;
		wxSpinCtrl* m_statcharspirit;
		wxStaticText* m_staticText113114;
		wxSpinCtrl* m_statcharmagicstone;
		wxStaticText* m_staticText113115;
		wxChoice* m_statcharabilityset;
		wxListBox* m_statcharabilitylist;
		wxChoice* m_statcharabilitychoice;
		wxStaticText* m_staticText165;
		wxSpinCtrl* m_statcharabilitycost;
		wxStaticText* m_staticText1971;
		wxStaticText* m_staticText1981;
		wxChoice* m_statcharcommand1;
		wxChoice* m_statcharcommand2;
		wxStaticText* m_staticText1991;
		wxStaticText* m_staticText2001;
		wxChoice* m_statcharcommandtrance1;
		wxChoice* m_statcharcommandtrance2;
		wxStaticText* m_staticText201;
		wxChoice* m_statchartranceattack;
		wxStaticText* m_staticText202;
		wxChoice* m_statcharequipset;
		wxStaticText* m_staticText203;
		wxStaticText* m_staticText204;
		wxChoice* m_statcharweapon;
		wxStaticText* m_staticText209;
		wxStaticText* m_staticText205;
		wxStaticText* m_staticText206;
		wxChoice* m_statcharhead;
		wxChoice* m_statcharwrist;
		wxStaticText* m_staticText207;
		wxStaticText* m_staticText208;
		wxChoice* m_statchararmor;
		wxChoice* m_statcharaccessory;
		wxScrolledWindow* m_statlvlscrolledwindow;
		wxStaticText* m_staticText11312;
		wxButton* m_statlvlexpmanage;
		wxListBox* m_statlvlexplist;
		wxSpinCtrl* m_statlvlexpspin;
		wxStaticText* m_staticText113121;
		wxButton* m_statlvlhpmanage;
		wxListBox* m_statlvlhplist;
		wxSpinCtrl* m_statlvlhpspin;
		wxStaticText* m_staticText113122;
		wxButton* m_statlvlmpmanage;
		wxListBox* m_statlvlmplist;
		wxSpinCtrl* m_statlvlmpspin;
		wxPanel* m_panel68;
		wxScrolledWindow* m_partyspecialavailabilityscrolledwindow;
		wxStaticText* m_staticText473;
		wxListBox* m_partyspecialmagicswordlist;
		wxChoice* m_partyspecialmagicswordspell;
		wxPanel* m_panelinventory;
		wxNotebook* m_notebookinventory;
		wxPanel* m_panel26;
		wxNotebook* m_notebookitem;
		wxPanel* m_panel27;
		wxScrolledWindow* m_itemscrolledwindow;
		wxStaticText* m_staticText155;
		wxButton* m_itemnamebutton;
		wxStaticText* m_staticText1551;
		wxButton* m_itemhelpbutton;
		wxStaticText* m_staticText15511;
		wxButton* m_itemhelp2button;
		wxStaticText* m_staticText15521;
		wxCheckBox* m_itemtype1;
		wxCheckBox* m_itemtype2;
		wxCheckBox* m_itemtype3;
		wxCheckBox* m_itemtype4;
		wxCheckBox* m_itemtype5;
		wxCheckBox* m_itemtype6;
		wxCheckBox* m_itemtype7;
		wxCheckBox* m_itemtype8;
		wxStaticText* m_staticText1552;
		wxSpinCtrl* m_itemprice;
		wxStaticText* m_staticText15522;
		wxCheckBox* m_itemcharavailability16;
		wxCheckBox* m_itemcharavailability15;
		wxCheckBox* m_itemcharavailability14;
		wxCheckBox* m_itemcharavailability13;
		wxCheckBox* m_itemcharavailability12;
		wxCheckBox* m_itemcharavailability11;
		wxCheckBox* m_itemcharavailability10;
		wxCheckBox* m_itemcharavailability9;
		wxCheckBox* m_itemcharavailability8;
		wxCheckBox* m_itemcharavailability7;
		wxCheckBox* m_itemcharavailability6;
		wxCheckBox* m_itemcharavailability5;
		wxCheckBox* m_itemcharavailability4;
		wxCheckBox* m_itemcharavailability3;
		wxCheckBox* m_itemcharavailability2;
		wxCheckBox* m_itemcharavailability1;
		wxStaticText* m_staticText15523;
		wxChoice* m_itemstatid;
		wxButton* m_itemstatidbutton;
		wxStaticText* m_itemstatinfo;
		wxStaticText* m_staticText15524;
		wxChoice* m_itemskill1;
		wxChoice* m_itemskill2;
		wxChoice* m_itemskill3;
		wxStaticText* m_staticText15525;
		wxListBox* m_itemmenuposlist;
		wxBitmapButton* m_itemmenuposbuttonup;
		wxBitmapButton* m_itemmenuposbuttondown;
		wxStaticText* m_staticText15526;
		wxListBox* m_itemequipposlist;
		wxBitmapButton* m_itemequipposbuttonup;
		wxBitmapButton* m_itemequipposbuttondown;
		wxStaticText* m_staticText15527;
		wxSpinCtrl* m_itemicon;
		wxStaticBitmap* m_itemiconpreview;
		wxStaticText* m_staticText15528;
		wxSpinCtrl* m_itemiconcolor;
		wxStaticText* m_itemusablelabel;
		wxPanel* m_itemusablepanel;
		wxStaticText* m_staticText192;
		wxScrolledWindow* m_itemusableeffecthelpwindow;
		wxStaticText* m_itemusableeffecthelp;
		wxStaticText* m_staticText193;
		wxSpinCtrl* m_itemusablepower;
		wxStaticText* m_staticText194;
		wxCheckBox* m_itemusablestatus1;
		wxCheckBox* m_itemusablestatus2;
		wxCheckBox* m_itemusablestatus3;
		wxCheckBox* m_itemusablestatus4;
		wxCheckBox* m_itemusablestatus5;
		wxCheckBox* m_itemusablestatus6;
		wxCheckBox* m_itemusablestatus7;
		wxCheckBox* m_itemusablestatus8;
		wxCheckBox* m_itemusablestatus9;
		wxCheckBox* m_itemusablestatus10;
		wxCheckBox* m_itemusablestatus11;
		wxCheckBox* m_itemusablestatus12;
		wxCheckBox* m_itemusablestatus13;
		wxCheckBox* m_itemusablestatus14;
		wxCheckBox* m_itemusablestatus15;
		wxCheckBox* m_itemusablestatus16;
		wxCheckBox* m_itemusablestatus17;
		wxCheckBox* m_itemusablestatus18;
		wxCheckBox* m_itemusablestatus19;
		wxCheckBox* m_itemusablestatus20;
		wxCheckBox* m_itemusablestatus21;
		wxCheckBox* m_itemusablestatus22;
		wxCheckBox* m_itemusablestatus23;
		wxCheckBox* m_itemusablestatus24;
		wxCheckBox* m_itemusablestatus25;
		wxCheckBox* m_itemusablestatus26;
		wxCheckBox* m_itemusablestatus27;
		wxCheckBox* m_itemusablestatus28;
		wxCheckBox* m_itemusablestatus29;
		wxCheckBox* m_itemusablestatus30;
		wxCheckBox* m_itemusablestatus31;
		wxCheckBox* m_itemusablestatus32;
		wxStaticText* m_staticText195;
		wxStaticText* m_itemusablemodel;
		wxButton* m_itemusablemodeledit;
		wxStaticText* m_staticText196;
		wxChoice* m_itemusabletargettype;
		wxStaticText* m_staticText198;
		wxChoice* m_itemusabletargetamount;
		wxStaticText* m_staticText197;
		wxChoice* m_itemusabletargetpriority;
		wxStaticText* m_staticText199;
		wxCheckBox* m_itemusabletargetflagdead;
		wxCheckBox* m_itemusabletargetflagdeadfirst;
		wxCheckBox* m_itemusabletargetflagcamera;
		wxStaticText* m_staticText200;
		wxChoice* m_itemusabletargetpanel;
		wxStaticText* m_itemweaponlabel;
		wxPanel* m_itemweaponpanel;
		wxStaticText* m_staticText156;
		wxChoice* m_itemweaponformula;
		wxStaticText* m_staticText157;
		wxSpinCtrl* m_itemweaponpower;
		wxStaticText* m_staticText158;
		wxCheckBox* m_itemweaponelement1;
		wxCheckBox* m_itemweaponelement2;
		wxCheckBox* m_itemweaponelement3;
		wxCheckBox* m_itemweaponelement4;
		wxCheckBox* m_itemweaponelement5;
		wxCheckBox* m_itemweaponelement6;
		wxCheckBox* m_itemweaponelement7;
		wxCheckBox* m_itemweaponelement8;
		wxStaticText* m_staticText159;
		wxChoice* m_itemweaponstatus;
		wxStaticText* m_staticText2961;
		wxSpinCtrl* m_itemweaponaccuracy;
		wxStaticText* m_staticText297;
		wxCheckBox* m_itemweaponflag1;
		wxCheckBox* m_itemweaponflag2;
		wxCheckBox* m_itemweaponflag3;
		wxCheckBox* m_itemweaponflag4;
		wxCheckBox* m_itemweaponflag5;
		wxCheckBox* m_itemweaponflag6;
		wxCheckBox* m_itemweaponflag7;
		wxCheckBox* m_itemweaponflag8;
		wxStaticText* m_staticText298;
		wxSpinCtrl* m_itemweaponunk1;
		wxStaticText* m_staticText299;
		wxSpinCtrl* m_itemweaponunk2;
		wxStaticText* m_staticText160;
		wxSpinCtrl* m_itemweaponmodel;
		wxStaticText* m_itemarmorlabel;
		wxPanel* m_itemarmorpanel;
		wxStaticText* m_staticText161;
		wxSpinCtrl* m_itemarmordefence;
		wxStaticText* m_staticText162;
		wxSpinCtrl* m_itemarmorevade;
		wxStaticText* m_staticText163;
		wxSpinCtrl* m_itemarmormagicdefence;
		wxStaticText* m_staticText164;
		wxSpinCtrl* m_itemarmormagicevade;
		wxPanel* m_panel28;
		wxScrolledWindow* m_keyitemscrolledwindow;
		wxStaticText* m_staticText1553;
		wxButton* m_keyitemnamebutton;
		wxStaticText* m_staticText15512;
		wxButton* m_keyitemhelpbutton;
		wxStaticText* m_staticText155111;
		wxButton* m_keyitemdescriptionbutton;
		wxPanel* m_panel9;
		wxNotebook* m_notebookshop;
		wxPanel* m_panel32;
		wxScrolledWindow* m_shopscrolledwindow;
		wxStaticText* m_shopitemlistlabel;
		wxPanel* m_shopitemlistpanel;
		wxButton* m_shopitemadd;
		wxButton* m_shopitemremove;
		wxStaticText* m_staticText2711;
		wxListBox* m_shopitemfulllist;
		wxListBox* m_shopitemlist;
		wxBitmapButton* m_shopmoveitemup;
		wxBitmapButton* m_shopmoveitemdown;
		wxPanel* m_panel34;
		wxScrolledWindow* m_synthshopscrolledwindow;
		wxStaticText* m_staticText188;
		wxChoice* m_synthshopsynthitem;
		wxStaticText* m_staticText189;
		wxChoice* m_synthshoprecipe1;
		wxChoice* m_synthshoprecipe2;
		wxStaticText* m_staticText190;
		wxSpinCtrl* m_synthshopprice;
		wxStaticText* m_staticText191;
		wxCheckBox* m_synthshopsynthshop1;
		wxCheckBox* m_synthshopsynthshop2;
		wxCheckBox* m_synthshopsynthshop3;
		wxCheckBox* m_synthshopsynthshop4;
		wxCheckBox* m_synthshopsynthshop5;
		wxCheckBox* m_synthshopsynthshop6;
		wxCheckBox* m_synthshopsynthshop7;
		wxCheckBox* m_synthshopsynthshop8;
		wxPanel* m_panelbattle;
		wxScrolledWindow* m_enemyscrolledwindow;
		wxStaticText* m_enemystatlistlabel;
		wxListBox* m_enemystatlist;
		wxStaticText* m_enemyspelllistlabel;
		wxListBox* m_enemyspelllist;
		wxStaticText* m_enemygrouplistlabel;
		wxListBox* m_enemygrouplist;
		wxStaticText* m_staticText86;
		wxListBox* m_enemytextlist;
		wxStaticText* m_staticText1912;
		wxButton* m_enemyscriptedit;
		wxButton* m_enemyscriptentryedit;
		wxStaticText* m_staticText216;
		wxChoice* m_enemyscene;
		wxStaticText* m_staticText76;
		wxCheckBox* m_enemyflag1;
		wxCheckBox* m_enemyflag2;
		wxCheckBox* m_enemyflag3;
		wxCheckBox* m_enemyflag4;
		wxCheckBox* m_enemyflag5;
		wxCheckBox* m_enemyflag6;
		wxCheckBox* m_enemyflag7;
		wxCheckBox* m_enemyflag8;
		wxCheckBox* m_enemyflag9;
		wxCheckBox* m_enemyflag10;
		wxCheckBox* m_enemyflag11;
		wxCheckBox* m_enemyflag12;
		wxCheckBox* m_enemyflag13;
		wxCheckBox* m_enemyflag14;
		wxCheckBox* m_enemyflag15;
		wxCheckBox* m_enemyflag16;
		wxScrolledWindow* m_enemystatscrolledwindow;
		wxNotebook* m_enemystatbook;
		wxPanel* m_enemystatpanel1;
		wxStaticText* m_enemystatnamelabel;
		wxTextCtrl* m_enemystatname;
		wxButton* m_enemystatnamebutton;
		wxStaticText* m_enemystatlvllabel;
		wxSpinCtrl* m_enemystatlvl;
		wxStaticText* m_enemystathplabel;
		wxSpinCtrl* m_enemystathp;
		wxStaticText* m_enemystatmplabel;
		wxSpinCtrl* m_enemystatmp;
		wxStaticText* m_staticText69;
		wxSpinCtrl* m_enemystatattack;
		wxStaticText* m_enemystatspeedlabel;
		wxSpinCtrl* m_enemystatspeed;
		wxStaticText* m_enemystatstrengthlabel;
		wxSpinCtrl* m_enemystatstrength;
		wxStaticText* m_enemystatmagiclabel;
		wxSpinCtrl* m_enemystatmagic;
		wxStaticText* m_enemystatspiritlabel;
		wxSpinCtrl* m_enemystatspirit;
		wxStaticText* m_staticText65;
		wxSpinCtrl* m_enemystatdefence;
		wxStaticText* m_staticText66;
		wxSpinCtrl* m_enemystatevade;
		wxStaticText* m_staticText67;
		wxSpinCtrl* m_enemystatmagicdefence;
		wxStaticText* m_staticText68;
		wxSpinCtrl* m_enemystatmagicevade;
		wxStaticText* m_enemystatitemsteallabel;
		wxChoice* m_enemystatitemsteal1;
		wxChoice* m_enemystatitemsteal2;
		wxChoice* m_enemystatitemsteal3;
		wxChoice* m_enemystatitemsteal4;
		wxStaticText* m_enemystatitemdroplabel;
		wxChoice* m_enemystatitemdrop1;
		wxChoice* m_enemystatitemdrop2;
		wxChoice* m_enemystatitemdrop3;
		wxChoice* m_enemystatitemdrop4;
		wxStaticText* m_enemystatdropcardlabel;
		wxChoice* m_enemystatdropcard;
		wxStaticText* m_enemystatexplabel;
		wxSpinCtrl* m_enemystatexp;
		wxStaticText* m_enemystatgilslabel;
		wxSpinCtrl* m_enemystatgils;
		wxStaticText* m_staticText264;
		wxChoice* m_enemystatdefaultattack;
		wxStaticText* m_staticText71;
		wxChoice* m_enemystatmodel;
		wxPanel* m_enemystatpanel2;
		wxStaticText* m_enemystatelementabsorblabel;
		wxCheckBox* m_enemystatelementabsorb1;
		wxCheckBox* m_enemystatelementabsorb2;
		wxCheckBox* m_enemystatelementabsorb3;
		wxCheckBox* m_enemystatelementabsorb4;
		wxCheckBox* m_enemystatelementabsorb5;
		wxCheckBox* m_enemystatelementabsorb6;
		wxCheckBox* m_enemystatelementabsorb7;
		wxCheckBox* m_enemystatelementabsorb8;
		wxStaticText* m_enemystatelementimmunelabel;
		wxCheckBox* m_enemystatelementimmune1;
		wxCheckBox* m_enemystatelementimmune2;
		wxCheckBox* m_enemystatelementimmune3;
		wxCheckBox* m_enemystatelementimmune4;
		wxCheckBox* m_enemystatelementimmune5;
		wxCheckBox* m_enemystatelementimmune6;
		wxCheckBox* m_enemystatelementimmune7;
		wxCheckBox* m_enemystatelementimmune8;
		wxStaticText* m_enemystatelementhalflabel;
		wxCheckBox* m_enemystatelementhalf1;
		wxCheckBox* m_enemystatelementhalf2;
		wxCheckBox* m_enemystatelementhalf3;
		wxCheckBox* m_enemystatelementhalf4;
		wxCheckBox* m_enemystatelementhalf5;
		wxCheckBox* m_enemystatelementhalf6;
		wxCheckBox* m_enemystatelementhalf7;
		wxCheckBox* m_enemystatelementhalf8;
		wxStaticText* m_enemystatelementweaklabel;
		wxCheckBox* m_enemystatelementweak1;
		wxCheckBox* m_enemystatelementweak2;
		wxCheckBox* m_enemystatelementweak3;
		wxCheckBox* m_enemystatelementweak4;
		wxCheckBox* m_enemystatelementweak5;
		wxCheckBox* m_enemystatelementweak6;
		wxCheckBox* m_enemystatelementweak7;
		wxCheckBox* m_enemystatelementweak8;
		wxStaticText* m_enemystatclasslabel;
		wxCheckBox* m_enemystatclass1;
		wxCheckBox* m_enemystatclass2;
		wxCheckBox* m_enemystatclass3;
		wxCheckBox* m_enemystatclass4;
		wxCheckBox* m_enemystatclass5;
		wxCheckBox* m_enemystatclass6;
		wxCheckBox* m_enemystatclass7;
		wxCheckBox* m_enemystatclass8;
		wxStaticText* m_staticText79;
		wxCheckBox* m_enemystatstatusimmune1;
		wxCheckBox* m_enemystatstatusimmune2;
		wxCheckBox* m_enemystatstatusimmune3;
		wxCheckBox* m_enemystatstatusimmune4;
		wxCheckBox* m_enemystatstatusimmune5;
		wxCheckBox* m_enemystatstatusimmune6;
		wxCheckBox* m_enemystatstatusimmune7;
		wxCheckBox* m_enemystatstatusimmune8;
		wxCheckBox* m_enemystatstatusimmune9;
		wxCheckBox* m_enemystatstatusimmune10;
		wxCheckBox* m_enemystatstatusimmune11;
		wxCheckBox* m_enemystatstatusimmune12;
		wxCheckBox* m_enemystatstatusimmune13;
		wxCheckBox* m_enemystatstatusimmune14;
		wxCheckBox* m_enemystatstatusimmune15;
		wxCheckBox* m_enemystatstatusimmune16;
		wxCheckBox* m_enemystatstatusimmune17;
		wxCheckBox* m_enemystatstatusimmune18;
		wxCheckBox* m_enemystatstatusimmune19;
		wxCheckBox* m_enemystatstatusimmune20;
		wxCheckBox* m_enemystatstatusimmune21;
		wxCheckBox* m_enemystatstatusimmune22;
		wxCheckBox* m_enemystatstatusimmune23;
		wxCheckBox* m_enemystatstatusimmune24;
		wxCheckBox* m_enemystatstatusimmune25;
		wxCheckBox* m_enemystatstatusimmune26;
		wxCheckBox* m_enemystatstatusimmune27;
		wxCheckBox* m_enemystatstatusimmune28;
		wxCheckBox* m_enemystatstatusimmune29;
		wxCheckBox* m_enemystatstatusimmune30;
		wxCheckBox* m_enemystatstatusimmune31;
		wxCheckBox* m_enemystatstatusimmune32;
		wxStaticText* m_staticText78;
		wxCheckBox* m_enemystatstatusauto1;
		wxCheckBox* m_enemystatstatusauto2;
		wxCheckBox* m_enemystatstatusauto3;
		wxCheckBox* m_enemystatstatusauto4;
		wxCheckBox* m_enemystatstatusauto5;
		wxCheckBox* m_enemystatstatusauto6;
		wxCheckBox* m_enemystatstatusauto7;
		wxCheckBox* m_enemystatstatusauto8;
		wxCheckBox* m_enemystatstatusauto9;
		wxCheckBox* m_enemystatstatusauto10;
		wxCheckBox* m_enemystatstatusauto11;
		wxCheckBox* m_enemystatstatusauto12;
		wxCheckBox* m_enemystatstatusauto13;
		wxCheckBox* m_enemystatstatusauto14;
		wxCheckBox* m_enemystatstatusauto15;
		wxCheckBox* m_enemystatstatusauto16;
		wxCheckBox* m_enemystatstatusauto17;
		wxCheckBox* m_enemystatstatusauto18;
		wxCheckBox* m_enemystatstatusauto19;
		wxCheckBox* m_enemystatstatusauto20;
		wxCheckBox* m_enemystatstatusauto21;
		wxCheckBox* m_enemystatstatusauto22;
		wxCheckBox* m_enemystatstatusauto23;
		wxCheckBox* m_enemystatstatusauto24;
		wxCheckBox* m_enemystatstatusauto25;
		wxCheckBox* m_enemystatstatusauto26;
		wxCheckBox* m_enemystatstatusauto27;
		wxCheckBox* m_enemystatstatusauto28;
		wxCheckBox* m_enemystatstatusauto29;
		wxCheckBox* m_enemystatstatusauto30;
		wxCheckBox* m_enemystatstatusauto31;
		wxCheckBox* m_enemystatstatusauto32;
		wxStaticText* m_staticText80;
		wxCheckBox* m_enemystatstatusinitial1;
		wxCheckBox* m_enemystatstatusinitial2;
		wxCheckBox* m_enemystatstatusinitial3;
		wxCheckBox* m_enemystatstatusinitial4;
		wxCheckBox* m_enemystatstatusinitial5;
		wxCheckBox* m_enemystatstatusinitial6;
		wxCheckBox* m_enemystatstatusinitial7;
		wxCheckBox* m_enemystatstatusinitial8;
		wxCheckBox* m_enemystatstatusinitial9;
		wxCheckBox* m_enemystatstatusinitial10;
		wxCheckBox* m_enemystatstatusinitial11;
		wxCheckBox* m_enemystatstatusinitial12;
		wxCheckBox* m_enemystatstatusinitial13;
		wxCheckBox* m_enemystatstatusinitial14;
		wxCheckBox* m_enemystatstatusinitial15;
		wxCheckBox* m_enemystatstatusinitial16;
		wxCheckBox* m_enemystatstatusinitial17;
		wxCheckBox* m_enemystatstatusinitial18;
		wxCheckBox* m_enemystatstatusinitial19;
		wxCheckBox* m_enemystatstatusinitial20;
		wxCheckBox* m_enemystatstatusinitial21;
		wxCheckBox* m_enemystatstatusinitial22;
		wxCheckBox* m_enemystatstatusinitial23;
		wxCheckBox* m_enemystatstatusinitial24;
		wxCheckBox* m_enemystatstatusinitial25;
		wxCheckBox* m_enemystatstatusinitial26;
		wxCheckBox* m_enemystatstatusinitial27;
		wxCheckBox* m_enemystatstatusinitial28;
		wxCheckBox* m_enemystatstatusinitial29;
		wxCheckBox* m_enemystatstatusinitial30;
		wxCheckBox* m_enemystatstatusinitial31;
		wxCheckBox* m_enemystatstatusinitial32;
		wxStaticText* m_staticText701;
		wxChoice* m_enemystatbluemagic;
		wxScrolledWindow* m_enemyspellscrolledwindow;
		wxStaticText* m_staticText74;
		wxButton* m_enemyspellnamebutton;
		wxStaticText* m_enemyspelleffectlabel;
		wxScrolledWindow* m_enemyspelleffecthelpwindow;
		wxStaticText* m_enemyspelleffecthelp;
		wxStaticText* m_enemyspellpowerlabel;
		wxSpinCtrl* m_enemyspellpower;
		wxStaticText* m_enemyspellelementlabel;
		wxCheckBox* m_enemyspellelement1;
		wxCheckBox* m_enemyspellelement2;
		wxCheckBox* m_enemyspellelement3;
		wxCheckBox* m_enemyspellelement4;
		wxCheckBox* m_enemyspellelement5;
		wxCheckBox* m_enemyspellelement6;
		wxCheckBox* m_enemyspellelement7;
		wxCheckBox* m_enemyspellelement8;
		wxStaticText* m_enemyspellaccuracylabel;
		wxSpinCtrl* m_enemyspellaccuracy;
		wxStaticText* m_enemyspellstatuslabel;
		wxChoice* m_enemyspellstatus;
		wxButton* m_enemyspellstatusset;
		wxStaticText* m_enemyspellmplabel;
		wxSpinCtrl* m_enemyspellmp;
		wxStaticText* m_staticText75;
		wxButton* m_enemyspellanimedit;
		wxChoice* m_enemyspellanimreflect;
		wxStaticText* m_staticText244;
		wxChoice* m_enemyspellbaseanim;
		wxStaticText* m_staticText313;
		wxCheckBox* m_enemyspelltargetflagdead;
		wxCheckBox* m_enemyspelltargetflagdeadfirst;
		wxCheckBox* m_enemyspelltargetflagcamera;
		wxStaticText* m_enemyspellflaglabel;
		wxCheckBox* m_enemyspellflag1;
		wxCheckBox* m_enemyspellflag2;
		wxCheckBox* m_enemyspellflag3;
		wxCheckBox* m_enemyspellflag4;
		wxCheckBox* m_enemyspellflag5;
		wxCheckBox* m_enemyspellflag6;
		wxCheckBox* m_enemyspellflag7;
		wxCheckBox* m_enemyspellflag8;
		wxScrolledWindow* m_enemygroupscrolledwindow;
		wxStaticText* m_enemygroupfrequencelabel;
		wxSpinCtrl* m_enemygroupfrequence;
		wxStaticText* m_enemygroupamountlabel;
		wxSpinCtrl* m_enemygroupamount;
		wxStaticText* m_enemygroupaplabel;
		wxSpinCtrl* m_enemygroupap;
		wxStaticText* m_staticText246;
		wxSpinCtrl* m_enemygroupcamera;
		wxStaticText* m_enemygroupidlabel;
		wxChoice* m_enemygroupid1;
		wxCheckBox* m_enemygrouptargetable1;
		wxCheckBox* m_enemygrouplink1;
		wxStaticText* m_staticText222;
		wxSpinCtrl* m_enemygroupposx1;
		wxStaticText* m_staticText223;
		wxSpinCtrl* m_enemygroupposy1;
		wxStaticText* m_staticText224;
		wxSpinCtrl* m_enemygroupposz1;
		wxStaticText* m_staticText225;
		wxSpinCtrl* m_enemygroupangle1;
		wxChoice* m_enemygroupid2;
		wxCheckBox* m_enemygrouptargetable2;
		wxCheckBox* m_enemygrouplink2;
		wxStaticText* m_staticText2221;
		wxSpinCtrl* m_enemygroupposx2;
		wxStaticText* m_staticText2231;
		wxSpinCtrl* m_enemygroupposy2;
		wxStaticText* m_staticText2241;
		wxSpinCtrl* m_enemygroupposz2;
		wxStaticText* m_staticText2251;
		wxSpinCtrl* m_enemygroupangle2;
		wxChoice* m_enemygroupid3;
		wxCheckBox* m_enemygrouptargetable3;
		wxCheckBox* m_enemygrouplink3;
		wxStaticText* m_staticText2222;
		wxSpinCtrl* m_enemygroupposx3;
		wxStaticText* m_staticText2232;
		wxSpinCtrl* m_enemygroupposy3;
		wxStaticText* m_staticText2242;
		wxSpinCtrl* m_enemygroupposz3;
		wxStaticText* m_staticText2252;
		wxSpinCtrl* m_enemygroupangle3;
		wxChoice* m_enemygroupid4;
		wxCheckBox* m_enemygrouptargetable4;
		wxCheckBox* m_enemygrouplink4;
		wxStaticText* m_staticText2223;
		wxSpinCtrl* m_enemygroupposx4;
		wxStaticText* m_staticText2233;
		wxSpinCtrl* m_enemygroupposy4;
		wxStaticText* m_staticText2243;
		wxSpinCtrl* m_enemygroupposz4;
		wxStaticText* m_staticText2253;
		wxSpinCtrl* m_enemygroupangle4;
		wxPanel* m_paneltetramaster;
		wxNotebook* m_notebookcard;
		wxPanel* m_panel63;
		wxScrolledWindow* m_cardscrolledwindow;
		wxStaticText* m_staticText266;
		wxButton* m_cardnamebutton;
		wxStaticText* m_staticText461;
		wxSpinCtrl* m_cardpoints;
		wxPanel* m_cardstatpsxpanel;
		wxStaticText* m_staticText4622;
		wxStaticText* m_staticText4621;
		wxStaticText* m_staticText448;
		wxChoice* m_cardplayertype;
		wxStaticText* m_staticText4521;
		wxSpinCtrl* m_cardplayerattack;
		wxStaticText* m_staticText4511;
		wxSpinCtrl* m_cardplayerdefence;
		wxSpinCtrl* m_cardplayermagicdefence;
		wxStaticText* m_cardplayerpowerlabel;
		wxStaticText* m_staticText462;
		wxStaticText* m_staticText463;
		wxStaticText* m_staticText4482;
		wxChoice* m_cardnpctype;
		wxStaticText* m_staticText452;
		wxSpinCtrl* m_cardnpcattack;
		wxStaticText* m_staticText451;
		wxSpinCtrl* m_cardnpcdefence;
		wxSpinCtrl* m_cardnpcmagicdefence;
		wxStaticText* m_cardnpcpowerlabel;
		wxStaticText* m_staticText464;
		wxStaticText* m_staticText465;
		wxPanel* m_cardstatsteampanel;
		wxStaticText* m_staticText4483;
		wxChoice* m_cardtype;
		wxStaticText* m_staticText45211;
		wxSpinCtrl* m_cardattack;
		wxStaticText* m_staticText45111;
		wxSpinCtrl* m_carddefence;
		wxSpinCtrl* m_cardmagicdefence;
		wxStaticText* m_cardpowerlabel;
		wxPanel* m_panel64;
		wxScrolledWindow* m_carddeckscrolledwindow;
		wxStaticText* m_staticText466;
		wxSpinCtrl* m_carddeckdifficulty;
		wxStaticText* m_staticText4481;
		wxChoice* m_carddeckset;
		wxButton* m_carddecksetbutton;
		wxStaticText* m_carddecksetinfo1;
		wxStaticText* m_carddecksetinfo2;
		wxPanel* m_panelenvironment;
		wxNotebook* m_notebookenvironment;
		wxPanel* m_panel131;
		wxScrolledWindow* m_textscrolledwindow;
		wxStaticText* m_textdatalistlabel;
		wxListBox* m_textdatalist;
		wxStaticText* m_textexportlabel;
		wxButton* m_textexport;
		wxStaticText* m_textcharmaptexturelistlabel;
		wxButton* m_textmanagecharmapbutton;
		wxStaticText* m_textcharmappallabel;
		wxListBox* m_textcharmaplist;
		wxChoice* m_textcharmappalchoice;
		wxStaticText* m_textcharmapexportlabel;
		wxButton* m_textcharmapexport;
		wxStaticText* m_textcharmapwindowlabel;
		wxScrolledWindow* m_textcharmapwindow;
		wxPanel* m_panel1311;
		wxNotebook* m_notebookworld;
		wxPanel* m_panel67;
		wxScrolledWindow* m_worldscrolledwindow;
		wxStaticText* m_staticText822;
		wxButton* m_worldscriptedit;
		wxButton* m_worldscriptentryedit;
		wxStaticText* m_staticText217;
		wxButton* m_worldpreload;
		wxStaticText* m_staticText1921;
		wxListBox* m_worldtextlist;
		wxStaticText* m_staticText831;
		wxButton* m_worldtextexport;
		wxStaticText* m_worldtextcharmaptexturelistlabel;
		wxButton* m_worldtextmanagecharmapbutton;
		wxStaticText* m_worldtextcharmappallabel;
		wxListBox* m_worldtextcharmaplist;
		wxChoice* m_worldtextcharmappalchoice;
		wxStaticText* m_worldtextcharmapexportlabel;
		wxButton* m_worldtextcharmapexport;
		wxStaticText* m_worldtextcharmapwindowlabel;
		wxScrolledWindow* m_worldtextcharmapwindow;
		wxPanel* m_panel681;
		wxScrolledWindow* m_worldplacescrolledwindow;
		wxStaticText* m_staticText8221;
		wxButton* m_worldplacenamebutton;
		wxPanel* m_panel69;
		wxScrolledWindow* m_worldbattlescrolledwindow;
		wxStaticText* m_staticText82211;
		wxPanel* m_worldbattlepanelspin;
		wxSpinCtrl* m_worldbattlebattlespin1;
		wxSpinCtrl* m_worldbattlebattlespin2;
		wxSpinCtrl* m_worldbattlebattlespin3;
		wxSpinCtrl* m_worldbattlebattlespin4;
		wxPanel* m_worldbattlepanelchoice;
		wxChoice* m_worldbattlebattlechoice1;
		wxChoice* m_worldbattlebattlechoice2;
		wxChoice* m_worldbattlebattlechoice3;
		wxChoice* m_worldbattlebattlechoice4;
		wxStaticText* m_worldbattlehelplabel1;
		wxStaticText* m_worldbattlehelplabel2;
		wxStaticText* m_worldbattlehelplabel3;
		wxStaticText* m_worldbattlehelplabel4;
		wxPanel* m_panel41;
		wxScrolledWindow* m_fieldscrolledwindow;
		wxStaticText* m_staticText1901;
		wxButton* m_fieldnamebutton;
		wxStaticText* m_staticText1911;
		wxButton* m_fieldeditscript;
		wxButton* m_fieldscriptentryedit;
		wxStaticText* m_staticText218;
		wxButton* m_fieldpreload;
		wxStaticText* m_staticText211;
		wxChoice* m_fieldtexturechoice;
		wxButton* m_fieldtexturemanage;
		wxStaticText* m_staticText210;
		wxScrolledWindow* m_fieldtexturepreview;
		wxPanel* m_panel17;
		wxScrolledWindow* m_battlescenescrolledwindow;
		wxStaticText* m_staticText821;
		wxButton* m_battlesceneexport;
		wxStaticText* m_staticText94;
		wxButton* m_battlesceneimport;
		wxStaticText* m_staticText93;
		wxPanel* m_battlesceneobjectpanel;
		wxButton* m_battlescenemanagetexture;
		wxStaticText* m_staticText981;
		wxListBox* m_battlescenetexturelist;
		wxChoice* m_battlescenetexturechoice;
		wxChoice* m_battlescenepalettechoice;
		wxStaticText* m_staticText95;
		wxScrolledWindow* m_battlescenetexture;
		wxPanel* m_panel171;
		wxScrolledWindow* m_spellanimscrolledwindow;
		wxStaticText* m_staticText8211;
		wxButton* m_spellanimsequence;
		wxPanel* m_panelffui;
		wxNotebook* m_notebookinterface;
		wxPanel* m_panel66;
		wxScrolledWindow* m_specialtextscrolledwindow;
		wxStaticText* m_staticText472;
		wxListBox* m_specialtextdatalist;
		wxPanel* m_panelmips;
		wxNotebook* m_notebookmips;
		wxPanel* m_panel661;
		wxScrolledWindow* m_mipsbattlegenscrolledwindow;
		wxStaticText* m_staticText47212;
		wxTextCtrl* m_mipsbattleramposgen;
		wxStaticText* m_staticText296;
		wxSpinCtrl* m_mipsbattleramlengthgen;
		wxStaticText* m_staticText472111;
		wxButton* m_mipsbattlecodegen;
		wxScrolledWindow* m_mipsbattlescrolledwindow;
		wxStaticText* m_staticText4721;
		wxTextCtrl* m_mipsbattlerampos;
		wxStaticText* m_staticText47211;
		wxButton* m_mipsbattlecode;
		wxPanel* m_panelcil;
		wxNotebook* m_notebookcil;
		wxPanel* m_panel6611;
		wxListBox* m_ciltypelist;
		wxListBox* m_cilmethodlist;
		wxScrolledWindow* m_cilmethodscrolledwindow;
		wxStaticText* m_staticText346;
		wxButton* m_cilmethodedit;
		wxPanel* m_panel92;
		wxListBox* m_cilmacrolist;
		wxScrolledWindow* m_cilmacroscrolledwindow;
		wxStaticText* m_staticText347;
		wxButton* m_cilmacrobutton;
		wxStaticText* m_staticText345;
		wxTextCtrl* m_cilmacrodescription;
		wxStaticText* m_staticText348;
		wxListBox* m_cilmacromethods;
		wxStaticText* m_staticText327;
		wxBoxSizer* m_cilmacroparametersizer;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNotebookMain( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnNotebookParty( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxSpell( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSpellChangeFlags( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickSpellModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickSpellModelAlt( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxSupport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSupportChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSupportChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSupportChangeHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSupportChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnListBoxCommand( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCommandChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCommandChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCommandChangeHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCommandChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxStat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatChangeDefaultName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnStatChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatChangeList( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxPartySpecial( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPartySpecialChangeList( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPartySpecialChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookInventory( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnNotebookNone( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxItem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeBattleHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeFlags( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnItemChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnItemPositionListClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnButtonClickItemModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxKeyItem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyItemChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyItemChangeHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyItemChangeDescription( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxShop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShopChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxSynthesisShop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShopChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShopChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnShopChangeFlags( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxEnemy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxEnemyStat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyStatRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnListBoxEnemySpell( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemySpellRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnListBoxEnemyGroup( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyGroupRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnListBoxEnemyText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyTextRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEnemyChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyChangeFlags( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyStatChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnemyChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnEnemySpellChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookCard( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxCard( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCardChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCardChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCardChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnCardChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxCardDeck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookEnvironment( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextEditText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnTextExportText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextManageCharmap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextCharmapListSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextCharmapPaletteChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextExportCharmap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextCharmapPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnListBoxWorldMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWorldChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxWorldPlace( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWorldChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxWorldBattle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWorldChangeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnWorldChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxField( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFieldChangeName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFieldChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFieldChangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFieldTexturePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnListBoxBattleScene( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneExportButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneImportButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneManageTexturesButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneTextureListBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneTextureRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneTextureChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleScenePaletteChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBattleSceneTexturePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnListBoxSpellAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpellAnimationChangeButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookInterface( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxSpecialText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpecialTextEditText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpecialTextRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnNotebookMips( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxMipsBattle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMipsBattleButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookCil( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnListBoxCilStruct( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxCilMethod( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCilMethodButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxCilMacro( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCilMacroButton( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxListBox* m_spelllist;
		wxTextCtrl* m_spellname;
		wxTextCtrl* m_spellhelp;
		wxChoice* m_spelleffect;
		wxSpinCtrl* m_spellpower;
		wxListBox* m_supportlist;
		wxTextCtrl* m_supportname;
		wxTextCtrl* m_supporthelp;
		wxListBox* m_cmdlist;
		wxTextCtrl* m_cmdname;
		wxTextCtrl* m_cmdhelp;
		wxListBox* m_statlist;
		wxTextCtrl* m_statchardefaultname;
		wxListBox* m_partyspeciallist;
		wxListBox* m_itemlist;
		wxTextCtrl* m_itemname;
		wxTextCtrl* m_itemhelp;
		wxTextCtrl* m_itemhelp2;
		wxChoice* m_itemusableeffect;
		wxListBox* m_keyitemlist;
		wxTextCtrl* m_keyitemname;
		wxTextCtrl* m_keyitemhelp;
		wxTextCtrl* m_keyitemdescription;
		wxListBox* m_shoplist;
		wxListBox* m_synthshoplist;
		wxListBox* m_enemylist;
		wxTextCtrl* m_enemyspellname;
		wxChoice* m_enemyspelleffect;
		wxListBox* m_cardlist;
		wxTextCtrl* m_cardname;
		wxListBox* m_carddecklist;
		wxListBox* m_textlist;
		wxListBox* m_worldlist;
		wxListBox* m_worldplacelist;
		wxTextCtrl* m_worldplacename;
		wxListBox* m_worldbattlelist;
		wxListBox* m_fieldlist;
		wxTextCtrl* m_fieldname;
		wxListBox* m_battlescenelist;
		wxListBox* m_spellanimlist;
		wxListBox* m_specialtextlist;
		wxListBox* m_mipsbattlelist;
		
		CDPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxTAB_TRAVERSAL ); 
		~CDPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DiscardableMessage
///////////////////////////////////////////////////////////////////////////////
class DiscardableMessage : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_label;
		wxButton* m_buttondiscard;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClickDiscard( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickOk( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DiscardableMessage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Warning"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 220,130 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DiscardableMessage();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SteamLanguageMessage
///////////////////////////////////////////////////////////////////////////////
class SteamLanguageMessage : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_label;
		wxButton* m_buttonok;
	
	public:
		wxChoice* m_choice;
		
		SteamLanguageMessage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Language"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 150,130 ), long style = wxCAPTION ); 
		~SteamLanguageMessage();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpellAnimationWindow
///////////////////////////////////////////////////////////////////////////////
class SpellAnimationWindow : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panelmain;
		wxPanel* m_panelall;
		wxStaticText* m_targetinfo;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_modelmain;
		wxChoice* m_modelall;
		wxCheckBox* m_enableall;
		
		SpellAnimationWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Choose Animation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,140 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SpellAnimationWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CurveEditorWindow
///////////////////////////////////////////////////////////////////////////////
class CurveEditorWindow : public wxDialog 
{
	private:
	
	protected:
		wxScrolledWindow* m_curvescrolled;
		wxCheckBox* m_linkup;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouse( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CurveEditorWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Stat Curve"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~CurveEditorWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpellStatusWindow
///////////////////////////////////////////////////////////////////////////////
class SpellStatusWindow : public wxDialog 
{
	private:
	
	protected:
		wxCheckBox* m_statusbox1;
		wxCheckBox* m_statusbox2;
		wxCheckBox* m_statusbox3;
		wxCheckBox* m_statusbox4;
		wxCheckBox* m_statusbox5;
		wxCheckBox* m_statusbox6;
		wxCheckBox* m_statusbox7;
		wxCheckBox* m_statusbox8;
		wxCheckBox* m_statusbox9;
		wxCheckBox* m_statusbox10;
		wxCheckBox* m_statusbox11;
		wxCheckBox* m_statusbox12;
		wxCheckBox* m_statusbox13;
		wxCheckBox* m_statusbox14;
		wxCheckBox* m_statusbox15;
		wxCheckBox* m_statusbox16;
		wxCheckBox* m_statusbox17;
		wxCheckBox* m_statusbox18;
		wxCheckBox* m_statusbox19;
		wxCheckBox* m_statusbox20;
		wxCheckBox* m_statusbox21;
		wxCheckBox* m_statusbox22;
		wxCheckBox* m_statusbox23;
		wxCheckBox* m_statusbox24;
		wxCheckBox* m_statusbox25;
		wxCheckBox* m_statusbox26;
		wxCheckBox* m_statusbox27;
		wxCheckBox* m_statusbox28;
		wxCheckBox* m_statusbox29;
		wxCheckBox* m_statusbox30;
		wxCheckBox* m_statusbox31;
		wxCheckBox* m_statusbox32;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SpellStatusWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Status Set"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~SpellStatusWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ItemStatWindow
///////////////////////////////////////////////////////////////////////////////
class ItemStatWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1671;
		wxStaticText* m_staticText1681;
		wxStaticText* m_staticText1691;
		wxStaticText* m_staticText1701;
		wxStaticText* m_staticText1711;
		wxStaticText* m_staticText1721;
		wxStaticText* m_staticText1731;
		wxStaticText* m_staticText1741;
		wxStaticText* m_staticText1751;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxSpinCtrl* m_strength;
		wxSpinCtrl* m_speed;
		wxSpinCtrl* m_magic;
		wxSpinCtrl* m_spirit;
		wxCheckBox* m_elementabsorb1;
		wxCheckBox* m_elementabsorb2;
		wxCheckBox* m_elementabsorb3;
		wxCheckBox* m_elementabsorb4;
		wxCheckBox* m_elementabsorb5;
		wxCheckBox* m_elementabsorb6;
		wxCheckBox* m_elementabsorb7;
		wxCheckBox* m_elementabsorb8;
		wxCheckBox* m_elementimmune1;
		wxCheckBox* m_elementimmune2;
		wxCheckBox* m_elementimmune3;
		wxCheckBox* m_elementimmune4;
		wxCheckBox* m_elementimmune5;
		wxCheckBox* m_elementimmune6;
		wxCheckBox* m_elementimmune7;
		wxCheckBox* m_elementimmune8;
		wxCheckBox* m_elementhalf1;
		wxCheckBox* m_elementhalf2;
		wxCheckBox* m_elementhalf3;
		wxCheckBox* m_elementhalf4;
		wxCheckBox* m_elementhalf5;
		wxCheckBox* m_elementhalf6;
		wxCheckBox* m_elementhalf7;
		wxCheckBox* m_elementhalf8;
		wxCheckBox* m_elementweak1;
		wxCheckBox* m_elementweak2;
		wxCheckBox* m_elementweak3;
		wxCheckBox* m_elementweak4;
		wxCheckBox* m_elementweak5;
		wxCheckBox* m_elementweak6;
		wxCheckBox* m_elementweak7;
		wxCheckBox* m_elementweak8;
		wxCheckBox* m_elementboost1;
		wxCheckBox* m_elementboost2;
		wxCheckBox* m_elementboost3;
		wxCheckBox* m_elementboost4;
		wxCheckBox* m_elementboost5;
		wxCheckBox* m_elementboost6;
		wxCheckBox* m_elementboost7;
		wxCheckBox* m_elementboost8;
		
		ItemStatWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Stat Set"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ItemStatWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CardSetWindow
///////////////////////////////////////////////////////////////////////////////
class CardSetWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_choicecard1;
		wxChoice* m_choicecard2;
		wxChoice* m_choicecard3;
		wxChoice* m_choicecard4;
		wxChoice* m_choicecard5;
		wxChoice* m_choicecard6;
		wxChoice* m_choicecard7;
		wxChoice* m_choicecard8;
		wxChoice* m_choicecard9;
		wxChoice* m_choicecard10;
		wxChoice* m_choicecard11;
		wxChoice* m_choicecard12;
		wxChoice* m_choicecard13;
		wxChoice* m_choicecard14;
		wxChoice* m_choicecard15;
		wxChoice* m_choicecard16;
		
		CardSetWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Card Set"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~CardSetWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PreferencesWindow
///////////////////////////////////////////////////////////////////////////////
class PreferencesWindow : public wxDialog 
{
	private:
	
	protected:
		wxAuiNotebook* m_auinotebook2;
		wxPanel* m_panel17;
		wxStaticText* m_staticText90;
		wxChoice* m_gamewindowcolor;
		wxStaticText* m_staticText218;
		wxChoice* m_textpreviewtype;
		wxPanel* m_panel172;
		wxStaticText* m_staticText334;
		wxChoice* m_steamlanguage;
		wxStaticText* m_staticText336;
		wxCheckBox* m_steamsinglelanguage;
		wxStaticText* m_staticText335;
		wxCheckBox* m_steamsaveus;
		wxCheckBox* m_steamsaveuk;
		wxCheckBox* m_steamsavejap;
		wxCheckBox* m_steamsaveger;
		wxCheckBox* m_steamsavefr;
		wxCheckBox* m_steamsaveit;
		wxCheckBox* m_steamsavesp;
		wxStaticText* m_staticText901;
		wxSpinCtrl* m_backgroundresolution;
		wxPanel* m_panel171;
		wxStaticText* m_staticText892;
		wxStaticText* m_staticText8911;
		wxTextCtrl* m_opcodechar;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSteamLanguageChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSingleLanguageMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_gamealphabet;
		
		PreferencesWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 350,300 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~PreferencesWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class IOHWSWindow
///////////////////////////////////////////////////////////////////////////////
class IOHWSWindow : public wxDialog 
{
	private:
	
	protected:
		wxCheckBox* m_spelltext;
		wxCheckBox* m_supporttext;
		wxCheckBox* m_commandtext;
		wxCheckBox* m_stattext;
		wxStaticText* m_staticText10514;
		wxCheckBox* m_itemtext;
		wxStaticText* m_staticText1051;
		wxCheckBox* m_enemytext;
		wxCheckBox* m_cardtext;
		wxStaticText* m_staticText10511;
		wxCheckBox* m_worldtext;
		wxCheckBox* m_fieldtext;
		wxStaticText* m_staticText10512;
		wxStaticText* m_staticText10513;
		wxStaticText* m_staticText10515;
		wxStaticText* m_staticText273;
		wxStaticText* m_staticText274;
		wxStaticText* m_staticText275;
		wxStaticText* m_staticText276;
		wxStaticText* m_staticText277;
		wxStaticText* m_staticText278;
		wxStaticText* m_staticText279;
		wxStaticText* m_staticText280;
		wxStaticText* m_staticText281;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChangeFile( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnCheckSection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_hwspicker;
		wxCheckBox* m_spell;
		wxCheckBox* m_support;
		wxCheckBox* m_command;
		wxCheckBox* m_stat;
		wxCheckBox* m_partyspecial;
		wxCheckBox* m_item;
		wxCheckBox* m_shop;
		wxCheckBox* m_enemy;
		wxCheckBox* m_card;
		wxCheckBox* m_text;
		wxCheckBox* m_world;
		wxCheckBox* m_field;
		wxCheckBox* m_scene;
		wxCheckBox* m_spellanim;
		wxCheckBox* m_ffui;
		wxCheckBox* m_assembly;
		wxCheckBox* m_enemylocal;
		wxCheckBox* m_worldlocal;
		wxCheckBox* m_fieldlocal;
		
		IOHWSWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~IOHWSWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ExportPPFWindow
///////////////////////////////////////////////////////////////////////////////
class ExportPPFWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxFilePickerCtrl* m_ppfpicker;
		wxCheckBox* m_undo;
		wxCheckBox* m_blockcheck;
		wxTextCtrl* m_description;
		
		ExportPPFWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export as ppf..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 350,205 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ExportPPFWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SaveSteamWindow
///////////////////////////////////////////////////////////////////////////////
class SaveSteamWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText309;
		wxStaticText* m_staticText3051;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxDirPickerCtrl* m_dirpicker;
		
		SaveSteamWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Create Steam Mod..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SaveSteamWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutWindow
///////////////////////////////////////////////////////////////////////////////
class AboutWindow : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_textCtrl13;
		wxButton* m_button18;
	
	public:
		
		AboutWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,390 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class HelpWindow
///////////////////////////////////////////////////////////////////////////////
class HelpWindow : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel171;
		wxTextCtrl* m_textCtrl174;
		wxPanel* m_panel17;
		wxTextCtrl* m_textCtrl17;
		wxPanel* m_panel18;
		wxTextCtrl* m_textCtrl171;
		wxPanel* m_panel19;
		wxTextCtrl* m_textCtrl172;
		wxPanel* m_panel26;
		wxTextCtrl* m_textCtrl28;
		wxPanel* m_panel201;
		wxTextCtrl* m_textCtrl1731;
		wxPanel* m_panel20;
		wxTextCtrl* m_textCtrl173;
		wxPanel* m_panel202;
		wxTextCtrl* m_textCtrl1732;
	
	public:
		wxListbook* m_helpbook;
		
		HelpWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Help"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,300 ), long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxRESIZE_BORDER ); 
		~HelpWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LogWindow
///////////////////////////////////////////////////////////////////////////////
class LogWindow : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_errornum;
		wxTextCtrl* m_errorctrl;
		wxTextCtrl* m_warningnum;
		wxTextCtrl* m_warningctrl;
		wxButton* m_buttonok;
	
	public:
		
		LogWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Log"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~LogWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class TextEditWindow
///////////////////////////////////////////////////////////////////////////////
class TextEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel14;
		wxButton* m_opcodeadd;
		wxButton* m_opcoderemove;
		wxListBox* m_opcodelist;
		wxBitmapButton* m_opcodeup;
		wxBitmapButton* m_opcodedown;
		wxButton* m_buttonset;
		wxButton* m_buttonpreview;
		wxButton* m_buttonbubble;
		wxTextCtrl* m_textcharmap;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnTextEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpcodeEdit( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_textctrl;
		wxSpinCtrl* m_sizex;
		wxSpinCtrl* m_sizey;
		PreviewTextCtrl* m_richtextctrl;
		
		TextEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Text"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,460 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~TextEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class TextOpcodeWindow
///////////////////////////////////////////////////////////////////////////////
class TextOpcodeWindow : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_opcode;
		wxStaticText* m_opcodehelp;
		wxPanel* m_opcodepaneldefault1;
		wxStaticText* m_defaultarglabel11;
		wxSpinCtrl* m_defaultarg11;
		wxPanel* m_opcodepaneldefault2;
		wxStaticText* m_defaultarglabel21;
		wxSpinCtrl* m_defaultarg21;
		wxStaticText* m_defaultarglabel22;
		wxSpinCtrl* m_defaultarg22;
		wxPanel* m_opcodepaneldefault3;
		wxStaticText* m_defaultarglabel31;
		wxSpinCtrl* m_defaultarg31;
		wxStaticText* m_defaultarglabel32;
		wxSpinCtrl* m_defaultarg32;
		wxStaticText* m_defaultarglabel33;
		wxSpinCtrl* m_defaultarg33;
		wxPanel* m_opcodepaneltokenize;
		wxStaticText* m_tokenizearglabel;
		wxSpinCtrl* m_tokenizearg;
		wxScrolledWindow* m_tokenizetextpanel;
		wxPanel* m_opcodepanel0x48;
		wxStaticText* m_0x48arglabel1;
		wxSpinCtrl* m_0x48arg1;
		wxGridSizer* m_0x48sizer;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOpcodeChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnArgumentChange( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnTokenMouse( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnTokenPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		TextOpcodeWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Text Opcode"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~TextOpcodeWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class TextExportWindow
///////////////////////////////////////////////////////////////////////////////
class TextExportWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_filepicker;
		wxChoice* m_strtype;
		wxTextCtrl* m_textheader;
		wxTextCtrl* m_textseparator;
		
		TextExportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export Text"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,340 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~TextExportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BatchExportWindow
///////////////////////////////////////////////////////////////////////////////
class BatchExportWindow : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_splitfilepanel;
		wxCheckBox* m_splitfile;
		wxPanel* m_scriptpanel;
		wxCheckBox* m_scriptsplitfile;
		wxCheckBox* m_scriptcomment;
		wxPanel* m_backgroundpanel;
		wxCheckBox* m_mergetile;
		wxChoice* m_languagetitle;
		wxCheckListBox* m_exportlist;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFilePick( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnSplitFileCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_filepicker;
		wxCheckBox* m_exportorder;
		
		BatchExportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export Data"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,355 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~BatchExportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BatchImportWindow
///////////////////////////////////////////////////////////////////////////////
class BatchImportWindow : public wxDialog 
{
	private:
	
	protected:
		wxCheckBox* m_fatalwarning;
		wxPanel* m_adjustsizepanel;
		wxCheckBox* m_adjustsize;
		wxCheckListBox* m_importlist;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFilePick( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnListRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_filepicker;
		
		BatchImportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import Data"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~BatchImportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CharmapTextureExportWindow
///////////////////////////////////////////////////////////////////////////////
class CharmapTextureExportWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxFilePickerCtrl* m_filepicker;
		
		CharmapTextureExportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export Texture"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,110 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~CharmapTextureExportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FieldTextureExportWindow
///////////////////////////////////////////////////////////////////////////////
class FieldTextureExportWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText328;
		wxStaticText* m_staticText326;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxFilePickerCtrl* m_filepicker;
		wxCheckBox* m_mergetiles;
		wxCheckBox* m_exportorder;
		wxCheckBox* m_onlyselected;
		wxChoice* m_languagetitle;
		
		FieldTextureExportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export Background"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,185 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~FieldTextureExportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BattleSceneExportWindow
///////////////////////////////////////////////////////////////////////////////
class BattleSceneExportWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxFilePickerCtrl* m_filepicker;
		wxCheckBox* m_usequads;
		
		BattleSceneExportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export Model"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,120 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~BattleSceneExportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BattleSceneImportWindow
///////////////////////////////////////////////////////////////////////////////
class BattleSceneImportWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxFilePickerCtrl* m_filepicker;
		
		BattleSceneImportWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import Model"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,105 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~BattleSceneImportWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BattleSceneImportLinkTextureWindow
///////////////////////////////////////////////////////////////////////////////
class BattleSceneImportLinkTextureWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText96;
		wxStaticText* m_staticText97;
		wxStaticText* m_staticText98;
		wxButton* m_buttonok;
	
	public:
		wxBoxSizer* m_sizerroot;
		wxScrolledWindow* m_linkscrolled;
		wxFlexGridSizer* m_linksizer;
		wxScrolledWindow* m_texturepanel;
		
		BattleSceneImportLinkTextureWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Set Textures"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,-1 ), long style = wxCAPTION|wxRESIZE_BORDER ); 
		~BattleSceneImportLinkTextureWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ManageTextureWindow
///////////////////////////////////////////////////////////////////////////////
class ManageTextureWindow : public wxDialog 
{
	private:
	
	protected:
		wxListBox* m_texturelist;
		wxListBox* m_palettelist;
		wxStaticText* m_staticText1021;
		wxSpinCtrl* m_texturepaletteselection;
		wxScrolledWindow* m_texturewindow;
		wxRadioBox* m_modifytyperadio;
		wxPanel* m_modifypaneldraw;
		wxPanel* m_drawpanelcolor;
		wxPanel* m_modifypanelimport;
		wxFilePickerCtrl* m_texturepicker;
		wxButton* m_buttonimport;
		wxStaticText* m_staticText99;
		wxStaticText* m_staticText100;
		wxSpinCtrl* m_textureposx;
		wxSpinCtrl* m_textureposy;
		wxStaticText* m_staticText101;
		wxStaticText* m_staticText102;
		wxSpinCtrl* m_texturewidth;
		wxSpinCtrl* m_textureheight;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnListBoxSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListBoxRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSpinPosition( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnTextureMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnTextureMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintTexture( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnModifyRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPaletteMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPalette( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnChooseFileImage( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ManageTextureWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Texture"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~ManageTextureWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ManageFieldTextureWindow
///////////////////////////////////////////////////////////////////////////////
class ManageFieldTextureWindow : public wxDialog 
{
	private:
	
	protected:
		wxCheckListBox* m_tilechecklist;
		wxListCtrl* m_animlist;
		wxStaticText* m_staticText1021;
		wxSpinCtrl* m_texturepaletteselection;
		wxScrolledWindow* m_texturewindow;
		wxRadioBox* m_modifytyperadio;
		wxPanel* m_modifypaneldraw;
		wxPanel* m_drawpanelcolor;
		wxPanel* m_modifypanelimport;
		wxFilePickerCtrl* m_texturepicker;
		wxButton* m_buttonimport;
		wxStaticText* m_staticText99;
		wxStaticText* m_staticText100;
		wxSpinCtrl* m_textureposx;
		wxSpinCtrl* m_textureposy;
		wxButton* m_buttonexport;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnTileSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTileButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSpinPosition( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnTextureMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnTextureMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintTexture( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnModifyRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPaletteMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPalette( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnChooseFileImage( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ManageFieldTextureWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Field Background"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~ManageFieldTextureWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptEditWindow
///////////////////////////////////////////////////////////////////////////////
class ScriptEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxGridBagSizer* topsizer;
		wxListCtrl* m_functionlist;
		wxStaticText* m_staticText291;
		wxNotebook* m_panelnotebook;
		wxPanel* m_panelcommon;
		wxStaticText* m_intvaluesignedintlabel;
		wxTextCtrl* m_intvaluesignedint;
		wxStaticText* m_intvaluesignedlonglabel;
		wxTextCtrl* m_intvaluesignedlong;
		wxStaticText* m_intvaluebase16label;
		wxTextCtrl* m_intvaluebase16;
		wxStaticText* m_intvalueitemlabel;
		wxTextCtrl* m_intvalueitem;
		wxStaticText* m_intvaluebuttonlabel;
		wxTextCtrl* m_intvaluebutton;
		wxPanel* m_panelbattle;
		wxStaticText* m_intvaluebase64label;
		wxTextCtrl* m_intvaluebase64;
		wxStaticText* m_intvalueattacklabel;
		wxTextCtrl* m_intvalueattack;
		wxStaticText* m_staticText305;
		wxTextCtrl* m_intvaluestatusa;
		wxStaticText* m_staticText3051;
		wxTextCtrl* m_intvaluestatusb;
		wxStaticText* m_intvaluespelllabel;
		wxTextCtrl* m_intvaluespell;
		wxStaticText* m_intvaluecmdlabel;
		wxTextCtrl* m_intvaluecmd;
		wxCheckBox* m_fielddisplaybackground;
		wxCheckBox* m_fielddisplaymesh;
		wxPanel* m_fielddisplaypanel;
		wxChoice* m_fieldcoordchoice;
		wxPanel* m_panelworld;
		wxPanel* m_worlddisplaypanel;
		wxChoice* m_worldcoordchoice;
		wxStaticBitmap* m_localvarshowimg;
		wxStaticText* m_staticText274;
		wxTextCtrl* m_localvartext;
		wxTextCtrl* m_scripttext;
		wxButton* m_functionlistbutton;
		wxStaticText* m_staticText197;
		wxScrolledWindow* m_argpanel;
		wxFlexGridSizer* argsizer;
		wxButton* m_buttoncancel;
		wxButton* m_buttonparse;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFunctionRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnFunctionChoose( wxListEvent& event ) { event.Skip(); }
		virtual void OnIntValueText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWorldMapPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnShowHideLocalVar( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnFunctionUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFunctionNewLine( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxPanel* m_panelfield;
		wxTextCtrl* m_helptextctrl;
		
		ScriptEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Script"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,650 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~ScriptEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptEditPropertiesWindow
///////////////////////////////////////////////////////////////////////////////
class ScriptEditPropertiesWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText3071;
		wxStaticText* m_staticText2461;
		wxStaticText* m_staticText2451;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
	
	public:
		wxChoice* m_entryctrl;
		wxSpinCtrl* m_entrytypectrl;
		wxSpinCtrl* m_typectrl;
		
		ScriptEditPropertiesWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 230,155 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ScriptEditPropertiesWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptEditEntryWindow
///////////////////////////////////////////////////////////////////////////////
class ScriptEditEntryWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttonadd;
		wxButton* m_buttonremove;
		wxListBox* m_entrylist;
		wxStaticText* m_staticText324;
		wxSpinCtrl* m_entrytype;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEntrySelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		
	
	public:
		
		ScriptEditEntryWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Entries"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ScriptEditEntryWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptHelpWindow
///////////////////////////////////////////////////////////////////////////////
class ScriptHelpWindow : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_searchtextctrl;
		wxStaticBitmap* m_searchbtn;
		wxAuiNotebook* m_listbook;
		wxPanel* m_panel94;
		wxListBox* m_listfunction;
		wxPanel* m_panel95;
		wxListBox* m_listvariable;
		wxPanel* m_panel951;
		wxListBox* m_listvarcode;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSearchButton( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnListClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListDoubleClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_helptextctrl;
		
		ScriptHelpWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Function List"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,400 ), long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxRESIZE_BORDER ); 
		~ScriptHelpWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AnimSequenceEditWindow
///////////////////////////////////////////////////////////////////////////////
class AnimSequenceEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxGridBagSizer* topsizer;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxScrolledWindow* m_sequencewindow;
		wxTextCtrl* m_helptextctrl;
		
		AnimSequenceEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Animation Sequence"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~AnimSequenceEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MipsScriptEditWindow
///////////////////////////////////////////////////////////////////////////////
class MipsScriptEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxGridBagSizer* topsizer;
		wxScrollBar* m_mipsscroll;
		wxButton* m_helpbutton;
		wxCheckBox* m_displaypos;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMipsScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnHelpClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddressCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxPanel* m_mipswindow;
		wxTextCtrl* m_codetextctrl;
		
		MipsScriptEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit MIPS Script"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,550 ), long style = wxCAPTION|wxCLOSE_BOX ); 
		~MipsScriptEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MipsScriptHelpWindow
///////////////////////////////////////////////////////////////////////////////
class MipsScriptHelpWindow : public wxDialog 
{
	private:
	
	protected:
		wxListBox* m_listfunction;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnListClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_helptextctrl;
		
		MipsScriptHelpWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("MIPS Help"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,400 ), long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxRESIZE_BORDER ); 
		~MipsScriptHelpWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CilScriptEditWindow
///////////////////////////////////////////////////////////////////////////////
class CilScriptEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_scriptposctrl;
		wxTextCtrl* m_scriptctrl;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CilScriptEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CIL Script"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~CilScriptEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImageMapEditWindow
///////////////////////////////////////////////////////////////////////////////
class ImageMapEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_objecttypechoice;
		wxButton* m_buttonadd;
		wxListBox* m_fulllist;
		wxStaticLine* m_staticline;
		wxButton* m_buttonremove;
		wxListBox* m_maplist;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnList( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ImageMapEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("File Associations"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ImageMapEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImageMapAddModelWindow
///////////////////////////////////////////////////////////////////////////////
class ImageMapAddModelWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText295;
		wxButton* m_buttonok;
	
	public:
		wxSpinCtrl* m_entryvalue;
		
		ImageMapAddModelWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add Model"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION ); 
		~ImageMapAddModelWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SteamImageMapEditWindow
///////////////////////////////////////////////////////////////////////////////
class SteamImageMapEditWindow : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_objecttypechoice;
		wxTextCtrl* m_objectlist;
		wxButton* m_buttoncancel;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SteamImageMapEditWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("File Associations"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 210,300 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~SteamImageMapEditWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ModManagerWindow
///////////////////////////////////////////////////////////////////////////////
class ModManagerWindow : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_textCtrl55;
		wxTreeListCtrl* m_listtree;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ModManagerWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Mod Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~ModManagerWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BackgroundEditorWindow
///////////////////////////////////////////////////////////////////////////////
class BackgroundEditorWindow : public wxDialog 
{
	private:
	
	protected:
		wxAuiNotebook* m_auinotebook;
		wxPanel* m_panelconverter;
		wxStaticText* m_staticText317;
		wxStaticText* m_staticText318;
		wxScrolledWindow* m_texturewindow;
		wxListBox* m_tilelist;
		wxStaticText* m_staticText314;
		wxStaticText* m_staticText328;
		wxPanel* m_panelmassconverter;
		wxStaticText* m_staticText3171;
		wxStaticText* m_staticText325;
		wxStaticText* m_staticText3181;
		wxStaticText* m_staticText3141;
		wxStaticText* m_staticText31411;
		wxPanel* m_panelimporter;
		wxStaticText* m_staticText316;
		wxStaticText* m_staticText319;
		wxButton* m_buttonapply;
		wxButton* m_buttonclose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFilePick( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnDirPick( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnRadioClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFieldChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPaintImage( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTileSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpinChange( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_imagepicker;
		wxDirPickerCtrl* m_exportdir;
		wxRadioButton* m_usegametilebtn;
		wxRadioButton* m_customtilebtn;
		wxChoice* m_fieldchoice;
		wxCheckBox* m_sortlayer;
		wxCheckBox* m_revertlayer;
		wxSpinCtrl* m_resolution;
		wxChoice* m_convertformat;
		wxChoice* m_dxtflagchoice;
		wxDirPickerCtrl* m_massimageimporter;
		wxTextCtrl* m_massimageformat;
		wxDirPickerCtrl* m_massexportdir;
		wxCheckBox* m_massfieldid;
		wxCheckBox* m_masssortlayer;
		wxCheckBox* m_massrevertlayer;
		wxSpinCtrl* m_massresolution;
		wxChoice* m_massconvertformat;
		wxChoice* m_massdxtflagchoice;
		wxFilePickerCtrl* m_importlauncher;
		wxDirPickerCtrl* m_importdir;
		
		BackgroundEditorWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Background Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,650 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~BackgroundEditorWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UnityViewerWindow
///////////////////////////////////////////////////////////////////////////////
class UnityViewerWindow : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_menufile;
		wxMenuItem* m_menureopen;
		wxMenu* m_menuassets;
		wxMenu* m_submenudata;
		wxMenu* m_submenufield;
		wxMenu* m_submenuaudio;
		wxMenu* m_submenulevel;
		wxMenu* m_submenushared;
		wxMenu* m_menuoptions;
		wxMenu* m_menuconvertimg;
		wxMenu* m_menuconvertimgquality;
		wxMenu* m_menuconvertaudio;
		wxMenu* m_menuconvertmodel;
		wxGauge* m_loadgauge;
		wxListCtrl* m_assetlist;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSortColumn( wxListEvent& event ) { event.Skip(); }
		virtual void OnAssetRightClick( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		wxMenuItem* m_menuexportpath;
		wxMenuItem* m_menuconvertimgnone;
		wxMenuItem* m_menuconvertimgbmp;
		wxMenuItem* m_menuconvertimgpng;
		wxMenuItem* m_menuconvertimgtga;
		wxMenuItem* m_menuconvertimgtiff;
		wxMenuItem* m_menuconvertimgqualitysame;
		wxMenuItem* m_menuconvertimgqualityalpha;
		wxMenuItem* m_menuconvertimgqualityrgb;
		wxMenuItem* m_menuconvertimgqualityrgba;
		wxMenuItem* m_menuconvertimgqualityargb;
		wxMenuItem* m_menuconvertimgqualitydxt1;
		wxMenuItem* m_menuconvertimgqualitydxt5;
		wxMenuItem* m_menuconvertimgqualitylow;
		wxMenuItem* m_menuconvertimgqualitymedium;
		wxMenuItem* m_menuconvertimgqualityhigh;
		wxMenuItem* m_menuconvertaudionone;
		wxMenuItem* m_menuconvertaudioakb;
		wxMenuItem* m_menuconvertmodelnone;
		wxMenuItem* m_menuconvertmodelfbxbin;
		wxMenuItem* m_menuconvertmodelfbxtext;
		wxMenuItem* m_menuconvertmodelautocad;
		wxMenuItem* m_menuconvertmodelcollada;
		wxMenuItem* m_menuconvertmodelwave;
		wxMenuItem* m_menuimportmodelexistingfiles;
		wxMenuItem* m_menuimportmodelmerge;
		wxMenuItem* m_menuimportmodelimportall;
		wxMenuItem* m_menuimportmodeldontflush;
		wxMenuItem* m_menuimportmodelflush;
		wxMenuItem* m_menuimportmodelmesh;
		wxMenuItem* m_menuimportmodelanims;
		wxMenuItem* m_menufolderx64;
		wxMenuItem* m_menufolderx86;
		
		UnityViewerWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Unity Assets Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~UnityViewerWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UnityLinkFileWindow
///////////////////////////////////////////////////////////////////////////////
class UnityLinkFileWindow : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText329;
		wxButton* m_buttonok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFileSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileInfoEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_message;
		wxChoice* m_existinglist;
		wxTextCtrl* m_fileinfo;
		
		UnityLinkFileWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Associated File"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,230 ), long style = wxCAPTION|wxSYSTEM_MENU ); 
		~UnityLinkFileWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UnityAddFileWindow
///////////////////////////////////////////////////////////////////////////////
class UnityAddFileWindow : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_buttoncancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxNotebook* m_filebook;
		wxButton* m_buttonadd;
		wxButton* m_buttonok;
		
		UnityAddFileWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Insert New Asset"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,340 ), long style = wxCAPTION|wxRESIZE_BORDER ); 
		~UnityAddFileWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UnityAddFilePanel
///////////////////////////////////////////////////////////////////////////////
class UnityAddFilePanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText333;
		wxStaticText* m_staticText332;
		wxStaticText* m_staticText334;
		wxButton* m_generateinfo;
		wxPanel* m_panel90;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFilePick( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileInfoEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_filepicker;
		wxChoice* m_filetype;
		wxTextCtrl* m_fileinternalname;
		wxTextCtrl* m_fileinfo;
		wxCheckBox* m_addbundleinfo;
		wxStaticText* m_filenamelabel;
		wxTextCtrl* m_filename;
		
		UnityAddFilePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~UnityAddFilePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MenuDEBUG
///////////////////////////////////////////////////////////////////////////////
class MenuDEBUG : public wxMenuBar 
{
	private:
	
	protected:
		wxMenu* m_menu6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDebugClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MenuDEBUG( long style = 0 );
		~MenuDEBUG();
	
};

#endif //__GUI_H__
