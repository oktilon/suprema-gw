#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "biostar2sdk/BS_Errno.h"
#include "biostar2sdk/BS_API.h"
#include "app.h"
#include "server.h"

using namespace std;

#define CARD_CODE_SZ        1024

static void onDeviceFound (BS2_DEVICE_ID deviceId);
static void onDeviceAccepted (BS2_DEVICE_ID deviceId);
static void onDeviceConnected (BS2_DEVICE_ID deviceId);
static void onDeviceDisconnected (BS2_DEVICE_ID deviceId);
static void onLogReceived (BS2_DEVICE_ID deviceId, const BS2Event* event);
static uint32_t dumpUsers (BS2_DEVICE_ID deviceId);
static uint32_t dumpDoors (BS2_DEVICE_ID deviceId);
static uint32_t dumpAccessGroups (BS2_DEVICE_ID deviceId);
static void dumpAccess (BS2_DEVICE_ID deviceId);
static void dumpDeviceInfo (BS2SimpleDeviceInfo info, BS2SimpleDeviceInfoEx ex);
static int isAcceptableUserId (const char *uid);
static void addDoor (BS2_DEVICE_ID deviceId, uint32_t id, string name);
static void addAccessGroup (BS2_DEVICE_ID deviceId, uint32_t id, string name, uint32_t levelId);
static void addAccess (BS2_DEVICE_ID deviceId, BS2SimpleDeviceInfo *info);
static void enrollUser (BS2_DEVICE_ID deviceId, BS2SimpleDeviceInfo *info, string id, uint64_t code, string pin, string name, uint32_t groupId);
static void clearDevice (BS2_DEVICE_ID deviceId);
static void onReadyToScanCard(BS2_DEVICE_ID id, uint32_t sequence);
static void readLog(BS2_DEVICE_ID id);

static void *serverContext = NULL;
static bool cleanOnConnect = false;
static uint32_t firstLog = 0;
static uint32_t maxLog = 100;

int server_init (bool clean, uint32_t logIndex, uint32_t logMax) {
    int r;

    cleanOnConnect = clean;
    firstLog = logIndex;
    maxLog = logMax;
    selfLogDbg ("BS2 version : %s", BS2_Version ());

    serverContext = BS2_AllocateContext ();

    r = BS2_InitializeEx (serverContext, "0.0.0.0");
    returnValIfFailErr (r == BS_SDK_SUCCESS, r, "Init error(%d): %s", r, server_error_message (r));

    r = BS2_SetDeviceEventListener (serverContext,
        onDeviceFound,
        onDeviceAccepted,
        onDeviceConnected,
        onDeviceDisconnected);
    returnValIfFailErr (r == BS_SDK_SUCCESS, r, "Set listeners error(%d): %s", r, server_error_message (r));

    r = BS2_SetAutoConnection (serverContext, 1);
    returnValIfFailErr (r == BS_SDK_SUCCESS, r, "Set auto-connection error(%d): %s", r, server_error_message (r));
    return r;
}

int server_run () {
    while (true) {
        usleep (100);
    }
    return 0;
}

static void onDeviceFound(BS2_DEVICE_ID deviceId) {
    selfLogInf ("onDeviceFound(%d)", deviceId);
}

static void onDeviceAccepted(BS2_DEVICE_ID deviceId) {
    selfLogInf ("onDeviceAccepted(%d)", deviceId);
}

static void onDeviceConnected(BS2_DEVICE_ID deviceId) {
    int r;
    BS2SimpleDeviceInfo deviceInfo = {0};
    BS2SimpleDeviceInfoEx deviceInfoEx = {0};
    selfLogInf ("onDeviceConnected(%d)", deviceId);

    r = BS2_StartMonitoringLog (serverContext, deviceId, onLogReceived);
    if (r != BS_SDK_SUCCESS) selfLogWrn ("Unable");

    r = BS2_GetDeviceInfoEx (serverContext, deviceId, &deviceInfo, &deviceInfoEx);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Get device info error(%d): %s", r, server_error_message (r));

    dumpDeviceInfo (deviceInfo, deviceInfoEx);

    if (firstLog < UINT32_MAX) {
        readLog (deviceId);
    }

    if (cleanOnConnect) {
        clearDevice (deviceId);
        addAccess (deviceId, &deviceInfo);
    }

    dumpAccess (deviceId);
}

static void onDeviceDisconnected(BS2_DEVICE_ID deviceId) {
    selfLogInf ("onDeviceDisconnected(%d)", deviceId);
}

static void onLogReceived (BS2_DEVICE_ID deviceId, const BS2Event* event) {
    char extra[128] = {0};
    if (event->code == 0x1102) {
        sprintf (extra, " (card:%08X)", atoi(event->userID));
    } else if ((event->code & 0xF000) == 0x1000) {
        sprintf (extra, " (user:%s)", event->userID);
    } else if ((event->code & 0xF000) == 0x2000 && event->userID[0] != 0) {
        sprintf (extra, " (user:%s)", event->userID);
    } else if ((event->code & 0xF000) == 0x5000) {
        sprintf (extra, " (door:%d)", event->doorID);
    }
    selfLogTrc ("Ev#%d [%s] %s [0x%04X]%s"
        , event->id
        , format_date (event->dateTime)
        , event_code (event->code)
        , event->code
        , extra
    );
}

static uint32_t dumpUsers (BS2_DEVICE_ID deviceId) {
    int r;
    uint32_t i, userCount, beg, end, j;
    char *users = NULL;
    char *id;
    char valid[128] = {0};
    char cards[1024];
    BS2UserBlob *data;
    BS2CSNCard card;

    r = BS2_GetUserList (serverContext, deviceId, &users, &userCount, isAcceptableUserId);
    returnValIfFailErr (r == BS_SDK_SUCCESS, 0, "Get user list error(%d): %s", r, server_error_message (r));

    selfLogDbg ("User count: %d", userCount);

    if (userCount) {
        data = (BS2UserBlob *) calloc (sizeof (BS2UserBlob), userCount);
        if (!data) {
            selfLogErr ("Memory allocate error(%d): %m", errno);
        } else {
            r = BS2_GetUserInfos (serverContext, deviceId, users, userCount, data);
            if (r != BS_SDK_SUCCESS) {
                selfLogErr ("Get user infos error(%d): %s", r, server_error_message (r));
            } else {
                for (i = 0; i < userCount; i++) {
                    id = users + i * BS2_USER_ID_SIZE;
                    beg = data[i].setting.startTime;
                    end = data[i].setting.endTime;
                    memset (cards, 0, 1024);

                    if (beg || end) {
                        strcat (valid, ", valid ");
                        if (beg) {
                            strcat (valid, "from ");
                            strcat (valid, format_date (beg));
                        }
                        if (end) {
                            strcat (valid, beg ? " to " : "to ");
                            strcat (valid, format_date (end));
                        }
                    }

                    for ( j = 0; j < data[i].user.numCards; j++) {
                        card = data[i].cardObjs[j];
                        if (cards[0] != 0) strcat (cards, ", ");
                        strcat (cards, card_data (card.data));
                    }

                    selfLogTrc ("User #%s [%s], cards:[%s], groups:[%s]%s"
                        , id
                        , data[i].user_name
                        , cards
                        , join_uint32_array (data[i].accessGroupId, ",", BS2_MAX_NUM_OF_ACCESS_GROUP_PER_USER)
                        , valid
                    );
                }
            }

            free (data);
        }
    }

    if (users) BS2_ReleaseObject (users);

    return userCount;
}

static uint32_t dumpDoors (BS2_DEVICE_ID deviceId) {
    int r;
    uint32_t i, doorCount;
    BS2Door *doors;

    r = BS2_GetAllDoor (serverContext, deviceId, &doors, &doorCount);
    returnValIfFailErr (r == BS_SDK_SUCCESS, 0, "Get door list error(%d): %s", r, server_error_message (r));

    selfLogDbg ("Door count: %d", doorCount);

    if (doorCount) {
        for (i = 0; i < doorCount; i++) {
            selfLogTrc ("Door %u [%s] relay:%u(%u) autoLock:%d, heldOpen:%d, lockFlags:%s, unlockFlags:%s, instant:%s, unconditional:%s"
                , doors[i].doorID
                , doors[i].name
                , doors[i].relay.deviceID
                , doors[i].relay.port
                , doors[i].autoLockTimeout
                , doors[i].heldOpenTimeout
                , door_flag (doors[i].lockFlags)
                , door_flag (doors[i].unlockFlags)
                , doors[i].instantLock ? "true" : "false"
                , doors[i].unconditionalLock ? "true" : "false"
            );
        }
        BS2_ReleaseObject (doors);
    }

    return doorCount;
}

static uint32_t dumpAccessGroups (BS2_DEVICE_ID deviceId) {
    int r;
    uint32_t groupsCount, i;
    BS2AccessGroup* groups = NULL;
    r = BS2_GetAllAccessGroup(serverContext, deviceId, &groups, &groupsCount);
    returnValIfFailErr (r == BS_SDK_SUCCESS, 0, "Get access groups error(%d): %s", r, server_error_message (r));

    selfLogDbg ("Access groups count: %d", groupsCount);
    for (i = 0; i < groupsCount; i++) {
        selfLogTrc ("Group %d [%s] levels: %s"
            , groups[i].id
            , groups[i].name
            , join_uint32_array (groups[i].accessLevels, ",", groups[i].numAccessLevels)
        );
    }

    if (groups) BS2_ReleaseObject (groups);

    return groupsCount;
}

static uint32_t dumpAccessLevels (BS2_DEVICE_ID deviceId) {
    int r;
    char buf[2048] = {0};
    char one[64] = {0};
    uint32_t levelsCount, i, j;
    BS2AccessLevel* levels = NULL;
    r = BS2_GetAllAccessLevel(serverContext, deviceId, &levels, &levelsCount);
    returnValIfFailErr (r == BS_SDK_SUCCESS, 0, "Get access levels error(%d): %s", r, server_error_message (r));

    selfLogDbg ("Access levels count: %d", levelsCount);
    for (i = 0; i < levelsCount; i++) {
        for (j = 0; j < levels[i].numDoorSchedules; j++) {
            snprintf (one, 63, "{d:%d, s:%d}"
                , levels[i].doorSchedules[j].doorID
                , levels[i].doorSchedules[j].scheduleID
            );
            if (buf[0] != 0) {
                strcat (buf, ", ");
            }
            strcat (buf, one);
        }
        selfLogTrc ("Level #%d (%s): [%s]"
            , levels[i].id
            , levels[i].name
            , buf
        );
    }

    if (levels) BS2_ReleaseObject (levels);

    return levelsCount;
}

static void dumpAccess (BS2_DEVICE_ID deviceId) {
    uint32_t cnt;

    cnt = dumpUsers (deviceId);
    cnt = dumpAccessGroups (deviceId);
    cnt = dumpAccessLevels (deviceId);
    cnt = dumpDoors (deviceId);

    (void) cnt;
}

static void dumpDeviceInfo (BS2SimpleDeviceInfo info, BS2SimpleDeviceInfoEx ex) {

    in_addr addr = {  info.ipv4Address };
    in_addr conn = {  info.connectedIP };

    selfLogTrc ("Device type        : %s", server_device_type ( info.type));
    selfLogTrc ("Connection mode    : %s", server_connection_mode ( info.connectionMode));
    selfLogTrc ("IP:Port            : %s:%d", inet_ntoa (addr),  info.port);
    selfLogTrc ("connectedIP        : %s", inet_ntoa (conn));
    selfLogTrc ("RS485 Mode         : %s", rs485_mode (info.rs485Mode));
    selfLogTrc ("maxNumOfUser       : %u", info.maxNumOfUser);
    selfLogTrc ("cardSupported      : %s", info.cardSupported ? "true" : "false");
    selfLogTrc ("pinSupported       : %s", info.pinSupported ? "true" : "false");
    selfLogTrc ("wlanSupported      : %s", info.wlanSupported ? "true" : "false");
    selfLogTrc ("dnsSupported       : %s", info.dnsSupported ? "true" : "false");
    selfLogTrc ("sslSupported       : %s", info.sslSupported ? "true" : "false");
    selfLogTrc ("rootCertExist      : %s", info.rootCertExist ? "true" : "false");
    selfLogTrc ("card1xSupported    : %s", info.card1xSupported ? "true" : "false");
    selfLogTrc ("systemExtSupported : %s", info.systemExtSupported ? "true" : "false");
    selfLogTrc ("cardExSupported    : %s", info.cardExSupported ? "true" : "false");
    selfLogTrc ("userNameSupported  : %s", info.userNameSupported ? "true" : "false");
    selfLogTrc ("userPhotoSupported : %s", info.userPhotoSupported ? "true" : "false");
    selfLogTrc ("fingerSupported    : %s", info.fingerSupported ? "true" : "false");
    selfLogTrc ("faceSupported      : %s", info.faceSupported ? "true" : "false");
    selfLogTrc ("tnaSupported       : %s", info.tnaSupported ? "true" : "false");
    selfLogTrc ("wiegandSupported   : %s", info.wiegandSupported ? "true" : "false");
    selfLogTrc ("imageLogSupported  : %s", info.imageLogSupported ? "true" : "false");
    selfLogTrc ("jobCodeSupported   : %s", info.jobCodeSupported ? "true" : "false");
    selfLogTrc ("dualIDSupported    : %s", info.dualIDSupported ? "true" : "false");
    selfLogTrc ("phraseSupported    : %s", info.phraseSupported ? "true" : "false");
    selfLogTrc ("card1xSupported    : %s", info.card1xSupported ? "true" : "false");
    selfLogTrc ("systemExtSupported : %s", info.systemExtSupported ? "true" : "false");
    selfLogTrc ("voipSupported      : %s", info.voipSupported ? "true" : "false");
    selfLogTrc ("rs485ExSupported   : %s", info.rs485ExSupported ? "true" : "false");
    selfLogTrc ("cardExSupported    : %s", info.cardExSupported ? "true" : "false");
    selfLogTrc ("useAlphanumericID  : %s", info.useAlphanumericID ? "true" : "false");
    selfLogTrc ("triggerActionSupported : %s", info.triggerActionSupported ? "true" : "false");
    selfLogTrc ("wiegandMultiSupported  : %s", info.wiegandMultiSupported ? "true" : "false");
    selfLogTrc ("supported          : %s", device_support_info (ex.supported));
}

static int isAcceptableUserId (const char *uid) {
    selfLogTrc ("user: %s", uid);
    return 1;
}

static void addDoor (BS2_DEVICE_ID deviceId, uint32_t id, string name) {
    int r;
    BS2Door door = { 0 };
    door.doorID = id;
    sprintf (door.name, "%s", name.c_str ());
    door.entryDeviceID = deviceId;
    door.exitDeviceID = 0;

    door.relay.deviceID = deviceId;
    door.relay.port = 0;

    door.autoLockTimeout = 4; // Lock it back
    door.heldOpenTimeout = 4; // Time to be opened
    door.instantLock = false;
    door.unlockFlags = BS2_DOOR_FLAG_EMERGENCY;
    door.lockFlags = BS2_DOOR_FLAG_NONE;
    door.unconditionalLock = false;

    r = BS2_SetDoor (serverContext, deviceId, &door, 1);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Add door error(%d): %s", r, server_error_message (r));

    selfLogInf ("Door %s [%d] added", door.name, id);
}

static void addAccessLevel (BS2_DEVICE_ID deviceId, uint32_t id, string name, uint32_t doorId) {
    int r;
    BS2AccessLevel accessLevel;

    accessLevel.id = id;
    sprintf (accessLevel.name, "%s", name.c_str ());
    accessLevel.doorSchedules[0].doorID = doorId;
    accessLevel.doorSchedules[0].scheduleID = 1; // Default always schedule
    accessLevel.numDoorSchedules = 1;

    r = BS2_SetAccessLevel (serverContext, deviceId, &accessLevel, 1);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Add access level error(%d): %s", r, server_error_message (r));

    selfLogInf ("Access level %s [%d] added", accessLevel.name, id);
}

static void addAccessGroup (BS2_DEVICE_ID deviceId, uint32_t id, string name, uint32_t levelId) {
    int r;
    BS2AccessGroup accessGroup;

    accessGroup.id = id;
    sprintf (accessGroup.name, "%s", name.c_str ());
    accessGroup.accessLevels[0] = levelId;
    accessGroup.numAccessLevels = 1;

    r = BS2_SetAccessGroup (serverContext, deviceId, &accessGroup, 1);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Add access group error(%d): %s", r, server_error_message (r));

    selfLogInf ("Access group %s [%d] added", accessGroup.name, id);
}

static void enrollUser (BS2_DEVICE_ID deviceId, BS2SimpleDeviceInfo *info, string id, uint64_t code, string pin, string name, uint32_t groupId) {
    int r, i, offset;
    bool pinAdded = false;
    uint32_t index, numCard;
    uint8_t byte;
    char buf[CARD_CODE_SZ] = {0};
    BS2UserSmallBlob userBlob = {0};
    BS2Card card;

    strcpy (userBlob.user.userID, id.c_str ());
    userBlob.user.flag = BS2_USER_FLAG_CREATED | BS2_USER_FLAG_UPDATED; // Create or Update
    userBlob.user.numCards = 0;
    userBlob.user.numFaces = 0;
    userBlob.user.numFingers = 0;

    userBlob.accessGroupId[0] = groupId;

    strcpy (reinterpret_cast<char*>(userBlob.user_name), name.c_str ());

    // userBlob.setting.startTime = time (NULL);
    // userBlob.setting.endTime = time (NULL) + 3600; // 1 hour
    if (pin.length() && info->pinSupported) {
        r = BS2_MakePinCode (serverContext, (char *)(pin.c_str ()), userBlob.pin);
        pinAdded = r == BS_SDK_SUCCESS;
        if (!pinAdded) selfLogWrn ("Create PIN error(%d): %s", r, server_error_message (r));
    }
    userBlob.setting.cardAuthMode = pinAdded ? BS2_AUTH_MODE_CARD_PIN : BS2_AUTH_MODE_CARD_ONLY;
    userBlob.setting.securityLevel = BS2_USER_SECURITY_LEVEL_DEFAULT; // BS2_USER_SECURITY_LEVEL_NORMAL

    // Scan 1 card
    if (info->cardSupported) {
        numCard = 1;
        userBlob.cardObjs = new BS2CSNCard[numCard];
        if (userBlob.cardObjs) {
            for (index = 0; index < numCard;) {
                memset(&card, 0x0, sizeof(card));
                memset(buf, 0x0, CARD_CODE_SZ);
                selfLogDbg ("Add card for [%s] %s", id.c_str (), name.c_str ());

                if (code) {
                    card.isSmartCard = 0;
                    card.card.type = BS2_CARD_TYPE_CSN;
                    card.card.size = BS2_CARD_DATA_SIZE;
                    for (i = 0; i < BS2_CARD_DATA_SIZE; i++) {
                        card.card.data[i] = 0;
                        if (i > 27) {
                            offset = BS2_CARD_DATA_SIZE - 1 - i;
                            byte = 0xFF & (code >> (offset * 8));
                            selfLogWrn ("Code 0x%08X offset=%d shift=%d byte=0x%02X", code, offset, offset * 8, byte);
                            card.card.data[i] = byte;
                        }
                    }
                    r = BS_SDK_SUCCESS;
                } else {
                    r = BS2_ScanCard(serverContext, deviceId, &card, onReadyToScanCard);
                }

                if (r != BS_SDK_SUCCESS) {
                    selfLogWrn ("Scan card error(%d): %s", r, server_error_message (r));
                } else {
                    if (card.isSmartCard) {
                        selfLogWrn ("CSN card only supported.");
                        continue;
                    }

                    memcpy(&userBlob.cardObjs[index], &card.card, sizeof(BS2CSNCard));
                    userBlob.user.numCards++;
                    index++;

                    for (i = 0; i < BS2_CARD_DATA_SIZE; i++) {
                        sprintf(buf + 3*i, "%02X ", card.card.data[i]);
                    }

                    selfLogTrc ("Added card type=%s, size=%d, isSmart={%d}, id=%u", server_card_type (card.card.type), card.card.size, card.isSmartCard, card.smartCard.cardID);
                    selfLogTrc ("card data={%s}", buf);
                }
            }
        }
    }


    r = BS2_EnrollUserSmall (serverContext
        , deviceId
        , &userBlob
        , 1 // Count
        , 1 // Overwrite
    );
    returnIfFailErr (r == BS_SDK_SUCCESS, "Enroll test user error(%d): %s", r, server_error_message (r));

    if (userBlob.cardObjs) delete[] userBlob.cardObjs;

    selfLogInf ("User [%s] %s enrolled!", id.c_str (), name.c_str ());
}

static void clearDevice (BS2_DEVICE_ID deviceId) {
    int r;

    r = BS2_RemoveAllUser (serverContext, deviceId);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Remove all users error(%d): %s", r, server_error_message (r));

    r = BS2_RemoveAllDoor (serverContext, deviceId);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Remove all doors error(%d): %s", r, server_error_message (r));

    r = BS2_RemoveAllAccessGroup (serverContext, deviceId);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Remove all access groups error(%d): %s", r, server_error_message (r));

    r = BS2_RemoveAllAccessLevel (serverContext, deviceId);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Remove all access levels error(%d): %s", r, server_error_message (r));

    selfLogTrc ("Device cleared!");
}

static void onReadyToScanCard(BS2_DEVICE_ID id, uint32_t sequence) {
    selfLogInf ("Place your card on the device. [%d, Seq: %d]", id, sequence);
}

static void readLog(BS2_DEVICE_ID deviceId) {
    int r;
    uint32_t logCount = 0, i;
    BS2Event *logs = NULL;

    r = BS2_GetLog (serverContext, deviceId, firstLog, maxLog, &logs, &logCount);
    returnIfFailErr (r == BS_SDK_SUCCESS, "Remove all access levels error(%d): %s", r, server_error_message (r));

    selfLogInf ("Read %d logs after %d", logCount, firstLog);
    for (i = 0; i < logCount; i++) {
        onLogReceived (deviceId, logs + i);
    }

    if (logs) BS2_ReleaseObject (logs);
}

static void addAccess (BS2_DEVICE_ID deviceId, BS2SimpleDeviceInfo *info) {
    addDoor (deviceId, 138, "first");
    addAccessLevel (deviceId, 100, "open", 138);
    addAccessGroup (deviceId, 1, "open", 100);
    // enrollUser (deviceId, info, "12003", 0x0C8238EC, "", "User Red", 1); // 0x0C8238EC
    enrollUser (deviceId, info, "12004", 0x491B07A4, "", "User Blue", 1); // 0x491B07A4
}

