#include <string>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "app.h"
#include "common.h"
#include "biostar2sdk/BS_Errno.h"
#include "biostar2sdk/BS_API.h"

using namespace std;

const char * server_error_message (int error) {
    switch (error) {
        case 1: return  "success";
        case 2: return  "duress success";
        case 3: return  "first auth success";
        case 4: return  "second auth success";
        case 5: return  "dual auth success";
        case 11: return  "wiegand bypass success";
        case 12: return  "anonymous success";
        case -1: return  "from device driver";
        case -101: return  "cannot open socket";
        case -102: return  "cannot connect socket";
        case -103: return  "cannot listen socket";
        case -104: return  "cannot accept socket";
        case -105: return  "cannot read socket";
        case -106: return  "cannot write socket";
        case -107: return  "socket is not connected";
        case -108: return  "socket is not open";
        case -109: return  "socket is not listened";
        case -110: return  "socket in progress";
        case -111: return  "ipv4 is not enable";
        case -112: return  "ipv6 is not enable";
        case -113: return  "not supported specified device info";
        case -114: return  "not enougth buffer";
        case -115: return  "not supported ipv6";
        case -116: return  "invalid address";
        case -200: return  "invalid param";
        case -201: return  "invalid packet";
        case -202: return  "invalid device id";
        case -203: return  "invalid device type";
        case -204: return  "packet checksum";
        case -205: return  "packet index";
        case -206: return  "packet command";
        case -207: return  "packet sequence";
        case -209: return  "no packet";
        case -210: return  "invalid code sign";
        case -300: return  "extraction fail";
        case -301: return  "verify fail";
        case -302: return  "identify fail";
        case -303: return  "identify timeout";
        case -304: return  "fingerprint capture fail";
        case -305: return  "fingerprint scan timeout";
        case -306: return  "fingerprint scan cancelled";
        case -307: return  "not same fingerprint";
        case -308: return  "extraction low quality";
        case -309: return  "capture low quality";
        case -310: return  "cannot find fingerprint";
        case -311: return  "fake finger detected";
        case -312: return  "fake finger try again";
        case -313: return  "fake finger sensor error";
        case -314: return  "cannot find face";
        case -315: return  "face capture fail";
        case -316: return  "face scan timeout";
        case -317: return  "face scan cancelled";
        case -318: return  "face scan failed";
        case -319: return  "unmasked face detected";
        case -320: return  "fake face detected";
        case -321: return  "cannot estimate";
        case -322: return  "normalize face";
        case -323: return  "small detection";
        case -324: return  "large detection";
        case -325: return  "biased detection";
        case -326: return  "rotated face";
        case -327: return  "overlapped face";
        case -328: return  "unopened eyes";
        case -329: return  "not looking front";
        case -330: return  "occluded mouth";
        case -331: return  "match fail";
        case -332: return  "incompatible face";
        case -400: return  "cannot open dir";
        case -401: return  "cannot open file";
        case -402: return  "cannot write file";
        case -403: return  "cannot seek file";
        case -404: return  "cannot read file";
        case -405: return  "cannot get stat";
        case -406: return  "cannot get sysinfo";
        case -407: return  "data mismatch";
        case -408: return  "already open dir";
        case -500: return  "invalid relay";
        case -501: return  "cannot write io packet";
        case -502: return  "cannot read io packet";
        case -503: return  "cannot read input";
        case -504: return  "read input timeout";
        case -505: return  "cannot enable input";
        case -506: return  "cannot set input duration";
        case -507: return  "invalid port";
        case -508: return  "invalid interphone type";
        case -510: return  "invalid lcd param";
        case -511: return  "cannot write lcd packet";
        case -512: return  "cannot read lcd packet";
        case -513: return  "invalid lcd packet";
        case -520: return  "input queue full";
        case -521: return  "wiegand queue full";
        case -522: return  "misc input queue full";
        case -523: return  "wiegand data queue full";
        case -524: return  "wiegand data queue empty";
        case -600: return  "not supported";
        case -601: return  "timeout";
        case -602: return  "cannot set time";
        case -700: return  "invalid data file";
        case -701: return  "too large data for slot";
        case -702: return  "invalid slot no";
        case -703: return  "invalid slot data";
        case -704: return  "cannot init db";
        case -705: return  "duplicate id";
        case -706: return  "user full";
        case -707: return  "duplicate template";
        case -708: return  "fingerprint full";
        case -709: return  "duplicate card";
        case -710: return  "card full";
        case -711: return  "no valid hdr file";
        case -712: return  "invalid log file";
        case -714: return  "cannot find user";
        case -715: return  "access level full";
        case -716: return  "invalid user id";
        case -717: return  "blacklist full";
        case -718: return  "user name full";
        case -719: return  "user image full";
        case -720: return  "user image size too big";
        case -721: return  "slot data checksum";
        case -722: return  "cannot update fingerprint";
        case -723: return  "template format mismatch";
        case -724: return  "no admin user";
        case -725: return  "cannot find log";
        case -726: return  "door schedule full";
        case -727: return  "db slot full";
        case -728: return  "access group full";
        case -729: return  "floor level full";
        case -730: return  "access schedule full";
        case -731: return  "holiday group full";
        case -732: return  "holiday full";
        case -733: return  "time period full";
        case -734: return  "no credential";
        case -735: return  "no biometric credential";
        case -736: return  "no card credential";
        case -737: return  "no pin credential";
        case -738: return  "no biometric pin credential";
        case -739: return  "no user name";
        case -740: return  "no user image";
        case -741: return  "reader full";
        case -742: return  "cache missed";
        case -743: return  "operator full";
        case -744: return  "invalid link id";
        case -745: return  "timer canceled";
        case -746: return  "user job full";
        case -747: return  "cannot update face";
        case -748: return  "face full";
        case -749: return  "floor schedule full";
        case -750: return  "cannot find auth group";
        case -751: return  "auth group full";
        case -752: return  "user phrase full";
        case -753: return  "dst schedule full";
        case -754: return  "cannot find dst schedule";
        case -755: return  "invalid schedule";
        case -756: return  "cannot find operator";
        case -757: return  "duplicate fingerprint";
        case -758: return  "duplicate face";
        case -759: return  "no face credential";
        case -760: return  "no fingerprint credential";
        case -761: return  "no face pin credential";
        case -762: return  "no fingerprint pin credential";
        case -763: return  "user image ex full";
        case -800: return  "invalid config";
        case -801: return  "cannot open config file";
        case -802: return  "cannot read config file";
        case -803: return  "invalid config file";
        case -804: return  "invalid config data";
        case -805: return  "cannot write config file";
        case -806: return  "invalid config index";
        case -900: return  "cannot scan finger";
        case -901: return  "cannot scan card";
        case -902: return  "cannot open rtc";
        case -903: return  "cannot set rtc";
        case -904: return  "cannot get rtc";
        case -905: return  "cannot set led";
        case -906: return  "cannot open device driver";
        case -907: return  "cannot find device";
        case -908: return  "cannot scan face";
        case -910: return  "slave full";
        case -911: return  "cannot add device";
        case -1000: return  "cannot find door";
        case -1001: return  "door full";
        case -1002: return  "cannot lock door";
        case -1003: return  "cannot unlock door";
        case -1004: return  "cannot release door";
        case -1005: return  "cannot find lift";
        case -1006: return  "lift full";
        case -1100: return  "access rule violation";
        case -1101: return  "disabled";
        case -1102: return  "not yet valid";
        case -1103: return  "expired";
        case -1104: return  "blacklist";
        case -1105: return  "cannot find access group";
        case -1106: return  "cannot find access level";
        case -1107: return  "cannot find access schedule";
        case -1108: return  "cannot find holiday group";
        case -1109: return  "cannot find blacklist";
        case -1110: return  "auth timeout";
        case -1111: return  "dual auth timeout";
        case -1112: return  "invalid auth mode";
        case -1113: return  "auth unexpected user";
        case -1114: return  "auth unexpected credential";
        case -1115: return  "dual auth fail";
        case -1116: return  "biometric auth required";
        case -1117: return  "card auth required";
        case -1118: return  "pin auth required";
        case -1119: return  "biometric or pin auth required";
        case -1120: return  "tna code required";
        case -1121: return  "auth server match refusal";
        case -1122: return  "cannot find floor level";
        case -1123: return  "auth fail";
        case -1124: return  "auth group required";
        case -1125: return  "identification required";
        case -1126: return  "anti tailgate violation";
        case -1127: return  "high temperature violation";
        case -1128: return  "cannot measure temperature";
        case -1129: return  "unmasked face violation";
        case -1130: return  "mask check required";
        case -1131: return  "thermal check required";
        case -1132: return  "face auth required";
        case -1133: return  "fingerprint auth required";
        case -1134: return  "face or pin auth required";
        case -1135: return  "fingerprint or pin auth required";
        case -1200: return  "cannot find zone";
        case -1201: return  "zone full";
        case -1202: return  "hard apb violation";
        case -1203: return  "soft apb violation";
        case -1204: return  "hard timed apb violation";
        case -1205: return  "soft timed apb violation";
        case -1206: return  "scheduled lock violation";
        case -1207: return  "intrusion alarm violation";
        case -1208: return  "set fire alarm";
        case -1209: return  "timed apb zone full";
        case -1210: return  "fire alarm zone full";
        case -1211: return  "scheduled lock unlock zone full";
        case -1212: return  "inactive zone";
        case -1213: return  "intrusion alarm zone full";
        case -1214: return  "cannot arm";
        case -1215: return  "cannot disarm";
        case -1216: return  "cannot find arm card";
        case -1217: return  "hard entrance limit count violation";
        case -1218: return  "soft entrance limit count violation";
        case -1219: return  "hard entrance limit time violation";
        case -1220: return  "soft entrance limit time violation";
        case -1221: return  "interlock zone door violation";
        case -1222: return  "interlock zone input violation";
        case -1223: return  "interlock zone full";
        case -1224: return  "auth limit schedule violation";
        case -1225: return  "auth limit count violation";
        case -1226: return  "auth limit user violation";
        case -1227: return  "soft auth limit violation";
        case -1228: return  "hard auth limit violation";
        case -1229: return  "lift lock unlock zone full";
        case -1230: return  "lift lock violation";
        case -1300: return  "card io";
        case -1301: return  "card init fail";
        case -1302: return  "card not activated";
        case -1303: return  "card cannot read data";
        case -1304: return  "card cis crc";
        case -1305: return  "card cannot write data";
        case -1306: return  "card read timeout";
        case -1307: return  "card read cancelled";
        case -1308: return  "card cannot send data";
        case -1310: return  "cannot find card";
        case -1400: return  "invalid password";
        case -1500: return  "camera init fail";
        case -1501: return  "jpeg encoder init fail";
        case -1502: return  "cannot encode jpeg";
        case -1503: return  "jpeg encoder not initialized";
        case -1504: return  "jpeg encoder deinit fail";
        case -1505: return  "camera capture fail";
        case -1506: return  "cannot detect face";
        case -2000: return  "file io";
        case -2002: return  "alloc mem";
        case -2003: return  "cannot upgrade";
        case -2004: return  "device locked";
        case -2005: return  "cannot send to server";
        case -2006: return  "cannot upgrade memoey";
        case -2007: return  "upgrade not supported";
        case -3000: return  "ssl init";
        case -3001: return  "ssl not supported";
        case -3002: return  "ssl cannot connect";
        case -3003: return  "ssl already connected";
        case -3004: return  "ssl invalid cert";
        case -3005: return  "ssl verify cert";
        case -3006: return  "ssl invalid key";
        case -3007: return  "ssl verify key";
        case -3100: return  "mobile portal";
        case -4001: return  "not osdp standard channel";
        case -4002: return  "already full slaves";
        case -4003: return  "duplicate osdp id";
        case -4004: return  "fail add osdp device";
        case -4005: return  "fail update osdp device";
        case -4006: return  "invalid osdp device id";
        case -4007: return  "fail master set key";
        case -4008: return  "fail slave set key";
        case -4009: return  "disconnect slave device";
        case -4010: return  "no license";
        case -4011: return  "license crc";
        case -4012: return  "license file not valid";
        case -4013: return  "license payload length";
        case -4014: return  "license parring json";
        case -4015: return  "license json format";
        case -4016: return  "license enable partial";
        case -4017: return  "license no match device";
        case -10000: return  "null pointer";
        case -10001: return  "uninitialized";
        case -10002: return  "cannot run service";
        case -10003: return  "canceled";
        case -10004: return  "exist";
        case -10005: return  "encrypt";
        case -10006: return  "decrypt";
        case -10007: return  "device busy";
        case -10008: return  "internal";
        case -10009: return  "invalid file format";
        case -10010: return  "invalid schedule id";
        case -10011: return  "unknown finger template";
        case -10100: return  "mobile status fault";
        case -10101: return  "mobile no response";
        case -10103: return  "mobile not login";
        case -10104: return  "mobile add device fault";
        case -10105: return  "mobile device not found";
        case -10201: return  "initialize filelog";
        case -10202: return  "initialize notifymgr";
        default: break;
    }
    return "Unknown";
}

const char * server_card_type (int type) {
    switch (type) {
        case 0x00: return "Unknown";
        case 0x01: return "CSN";
        case 0x02: return "Secure";
        case 0x03: return "Access";
        case 0x04: return "Csn mobile";
        case 0x05: return "Wiegand mobile";
        case 0x06: return "QR";
        case 0x07: return "Secure QR";
        case 0x0A: return "Wiegand";
        case 0x0B: return "Config card";
        case 0x0D: return "Custom smart";
        default: break;
    }
    return "Unknown";
}

const char * server_connection_mode (int type) {
    switch (type) {
        case 0: return "Server to device";
        case 1: return "Device to server";
        case 1078: return "MTU size min";
        case 1514: return "MTU size max";
        case 51213: return "SSL server port default";
        case 51212: return "server port default";
        case 51211: return "device port default";
        case 51210: return "UDP broadcast port default";
        case 51214: return "device zone master port default";
        case 52213: return "SSL server port default V6";
        case 52212: return "server port default V6";
        case 52211: return "device port default V6";
        case 52210: return "UDP broadcast port default V6";
        default: break;
    }
    return "Unknown";
}

const char * server_device_type (int type) {
    switch (type) {
        case 0x80: return "Rs485_slave";
        case 0x0E: return "Corestation_40";
        case 0x0F: return "Output_module";
        case 0x10: return "Input_module";
        case 0x11: return "Bioentry_p2";
        case 0x12: return "Biolite_n2";
        case 0x13: return "Xpass2";
        case 0x14: return "Xpass_s3";
        case 0x15: return "Bioentry_r2";
        case 0x16: return "Xpass_d2";
        case 0x17: return "Door_module_21";
        case 0x18: return "Xpass_d2_keypad";
        case 0x19: return "Facelite";
        case 0x1A: return "Xpass2_keypad";
        case 0x1B: return "Xpass_d2_rev";
        case 0x1C: return "Xpass_d2_keypad_rev";
        case 0x1D: return "Facestation_f2_fp";
        case 0x1E: return "Facestation_f2";
        case 0x1F: return "Xstation_2_qr";
        case 0x20: return "Xstation_2";
        case 0x21: return "Im_120";
        case 0x22: return "Xstation_2_fp";
        case 0x23: return "Biostation_3";
        case 0x24: return "3rd_osdp_device";
        case 0x25: return "3rd_osdp_io_device";
        case 0x26: return "Biostation_2a";
        default: break;
    }
    return "Unknown";
}

static inline void device_support_info_add (char *ret, int *first, const char *param) {
    if (*first) {
        *first = 0;
    } else {
        strcat (ret, ", ");
    }
    strcat (ret, param);
}

const char * device_support_info (int flags) {
    static char ret[512];
    int first = 1;

    memset (ret, 0, 512);

    if (flags & 0x00000001) device_support_info_add (ret, &first, "Rs485Ex");
    if (flags & 0x00000002) device_support_info_add (ret, &first, "iClass SEOS");
    if (flags & 0x00000004) device_support_info_add (ret, &first, "DST");
    if (flags & 0x00000008) device_support_info_add (ret, &first, "DesFire_ex");
    if (flags & 0x00000010) device_support_info_add (ret, &first, "Face_EX");
    if (flags & 0x00000020) device_support_info_add (ret, &first, "QR");
    if (flags & 0x00010000) device_support_info_add (ret, &first, "FingerScan");
    if (flags & 0x00020000) device_support_info_add (ret, &first, "FaceScan");
    if (flags & 0x00040000) device_support_info_add (ret, &first, "FaceExScan");
    if (flags & 0x00080000) device_support_info_add (ret, &first, "QR_Scan");
    if (flags & 0x01000000) device_support_info_add (ret, &first, "VoIP_Ex");

    return ret;
}

const char * event_code (uint16_t code) {
    static char ret[512];
    uint16_t mainCode = code & BS2_EVENT_MASK;
    uint16_t subCode = code & BS2_SUB_EVENT_MASK;

    memset (ret, 0, 512);

    switch (mainCode) {

        case BS2_EVENT_VERIFY_SUCCESS:
            strcpy (ret, "Verify success");
            switch (subCode) {
                case BS2_SUB_EVENT_VERIFY_ID_PIN: strcat (ret, " by id_pin"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FINGER: strcat (ret, " by id_finger"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FINGER_PIN: strcat (ret, " by id_finger_pin"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FACE: strcat (ret, " by id_face"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FACE_PIN: strcat (ret, " by id_face_pin"); break;
                case BS2_SUB_EVENT_VERIFY_CARD: strcat (ret, " by card"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_PIN: strcat (ret, " by card_pin"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FINGER: strcat (ret, " by card_finger"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FINGER_PIN: strcat (ret, " by card_finger_pin"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FACE: strcat (ret, " by card_face"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FACE_PIN: strcat (ret, " by card_face_pin"); break;
                case BS2_SUB_EVENT_VERIFY_AOC: strcat (ret, " by aoc"); break;
                case BS2_SUB_EVENT_VERIFY_AOC_PIN: strcat (ret, " by aoc_pin"); break;
                case BS2_SUB_EVENT_VERIFY_AOC_FINGER: strcat (ret, " by aoc_finger"); break;
                case BS2_SUB_EVENT_VERIFY_AOC_FINGER_PIN: strcat (ret, " by aoc_finger_pin"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FACE_FINGER: strcat (ret, " by card_face_finger"); break;
                case BS2_SUB_EVENT_VERIFY_CARD_FINGER_FACE: strcat (ret, " by card_finger_face"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FACE_FINGER: strcat (ret, " by id_face_finger"); break;
                case BS2_SUB_EVENT_VERIFY_ID_FINGER_FACE: strcat (ret, " by id_finger_face"); break;
                case BS2_SUB_EVENT_VERIFY_MOBLIE_CARD: strcat (ret, " by moblie_card"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_PIN: strcat (ret, " by mobile_card_pin"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FINGER: strcat (ret, " by mobile_card_finger"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FINGER_PIN: strcat (ret, " by mobile_card_finger_pin"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FACE: strcat (ret, " by mobile_card_face"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FACE_PIN: strcat (ret, " by mobile_card_face_pin"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FACE_FINGER: strcat (ret, " by mobile_card_face_finger"); break;
                case BS2_SUB_EVENT_VERIFY_MOBILE_CARD_FINGER_FACE: strcat (ret, " by mobile_card_finger_face"); break;
                default: break;
            }
            return ret;

        case BS2_EVENT_VERIFY_FAIL:
            strcpy (ret, "Verify fail");
            switch (subCode) {
                case BS2_SUB_EVENT_AUTH_FAIL_INVALID_AUTH_MODE: strcat (ret, ": invalid auth mode"); break;
                case BS2_SUB_EVENT_AUTH_FAIL_INVALID_CREDENTIAL: strcat (ret, ": invalid credential"); break;
                case BS2_SUB_EVENT_AUTH_FAIL_TIMEOUT: strcat (ret, ": timeout"); break;
                case BS2_SUB_EVENT_AUTH_FAIL_MATCHING_REFUSAL: strcat (ret, ": matching refusal"); break;
                default: break;
            }
            return ret;

        case BS2_EVENT_VERIFY_DURESS:
            strcat (ret, "Verify Duress");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_IDENTIFY_SUCCESS:
            strcat (ret, "Identify Success");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_IDENTIFY_FAIL:
            strcat (ret, "Identify Fail");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_IDENTIFY_DURESS:
            strcat (ret, "Identify Duress");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_DUAL_AUTH_SUCCESS:
            strcat (ret, "Dual Auth Success");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_DUAL_AUTH_FAIL:
            strcat (ret, "Dual Auth Fail");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_AUTH_FAILED:
            strcat (ret, "Auth Failed");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_ACCESS_DENIED:
            strcat (ret, "Access ");
            switch (subCode) {
                case BS2_SUB_EVENT_ACCESS_DENIED_ACCESS_GROUP: strcat (ret, "denied: Access group"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_DISABLED: strcat (ret, "denied: Disabled"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_EXPIRED: strcat (ret, "denied: Expired"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_ON_BLACKLIST: strcat (ret, "denied: On blacklist"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_APB: strcat (ret, "denied: Apb"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_TIMED_APB: strcat (ret, "denied: Timed apb"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_SCHEDULED_LOCK: strcat (ret, "denied: Scheduled lock"); break;
                case BS2_SUB_EVENT_ACCESS_EXCUSED_APB: strcat (ret, "excused: Apb"); break;
                case BS2_SUB_EVENT_ACCESS_EXCUSED_TIMED_APB: strcat (ret, "excused: Timed apb"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_FACE_DETECTION: strcat (ret, "denied: Face detection"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_CAMERA_CAPTURE: strcat (ret, "denied: Camera capture"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_FAKE_FINGER: strcat (ret, "denied: Fake finger"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_DEVICE_ZONE_ENTRANCE_LIMIT: strcat (ret, "denied: Device zone entrance limit"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_INTRUSION_ALARM: strcat (ret, "denied: Intrusion alarm"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_INTERLOCK: strcat (ret, "denied: Interlock"); break;
                case BS2_SUB_EVENT_ACCESS_EXCUSED_AUTH_LIMIT: strcat (ret, "excused: Auth limit"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_AUTH_LIMIT: strcat (ret, "denied: Auth limit"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_ANTI_TAILGATE: strcat (ret, "denied: Anti tailgate"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_HIGH_TEMPERATURE: strcat (ret, "denied: High temperature"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_NO_TEMPERATURE: strcat (ret, "denied: No temperature"); break;
                case BS2_SUB_EVENT_ACCESS_DENIED_UNMASKED_FACE: strcat (ret, "denied: Unmasked face"); break;
                default: strcat (ret, "denied"); break;
            }
            return ret;

        case BS2_EVENT_FAKE_FINGER_DETECTED:
            strcat (ret, "Fake_finger_detected");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_BYPASS_SUCCESS:
            strcat (ret, "Bypass_success");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_BYPASS_FAIL:
            strcat (ret, "Bypass_fail");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_ABNORMAL_TEMPERATURE_DETECTED:
            strcat (ret, "Abnormal_temperature_detected");
            switch (subCode) {
                default: break;
            }
            return ret;

        case BS2_EVENT_UNMASKED_FACE_DETECTED:
            strcat (ret, "Unmasked_face_detected");
            switch (subCode) {
                default: break;
            }
            return ret;


        // User
        case BS2_EVENT_USER_ENROLL_SUCCESS: strcat (ret, "Enroll success"); return ret;
        case BS2_EVENT_USER_ENROLL_FAIL: strcat (ret, "Enroll fail"); return ret;
        case BS2_EVENT_USER_UPDATE_SUCCESS: strcat (ret, "Update success"); return ret;
        case BS2_EVENT_USER_UPDATE_FAIL: strcat (ret, "Update fail"); return ret;
        case BS2_EVENT_USER_DELETE_SUCCESS: strcat (ret, "Delete success"); return ret;
        case BS2_EVENT_USER_DELETE_FAIL: strcat (ret, "Delete fail"); return ret;
        case BS2_EVENT_USER_DELETE_ALL_SUCCESS: strcat (ret, "Delete all success"); return ret;
        case BS2_EVENT_USER_ISSUE_AOC_SUCCESS: strcat (ret, "Issue aoc success"); return ret;
        case BS2_EVENT_USER_DUPLICATE_CREDENTIAL: strcat (ret, "Duplicate credential"); return ret;
        case BS2_EVENT_USER_UPDATE_PARTIAL_SUCCESS: strcat (ret, "Update partial success"); return ret;
        case BS2_EVENT_USER_UPDATE_PARTIAL_FAIL: strcat (ret, "Update partial fail"); return ret;
        case BS2_EVENT_USER_RELOADED: strcat (ret, "Reloaded"); return ret;

        // Device
        case BS2_EVENT_DEVICE_SYSTEM_RESET:
            strcat (ret, "System reset");
            return ret;
        case BS2_EVENT_DEVICE_SYSTEM_ERROR_OPENGL:
            strcat (ret, "System error OpenGL");
            return ret;
        case BS2_EVENT_DEVICE_SYSTEM_STARTED:
            strcat (ret, "System started");
            return ret;
        case BS2_EVENT_DEVICE_TIME_SET:
            strcat (ret, "Time set");
            if (code == BS2_EVENT_DEVICE_TIMEZONE_SET) strcat (ret, "+Timezone");
            if (code == BS2_EVENT_DEVICE_DST_SET) strcat (ret, "+DST");
            return ret;
        case BS2_EVENT_DEVICE_LINK_CONNECTED:
            strcat (ret, "Link connected");
            return ret;
        case BS2_EVENT_DEVICE_LINK_DISCONNETED:
            strcat (ret, "Link disconneted");
            return ret;
        case BS2_EVENT_DEVICE_DHCP_SUCCESS:
            strcat (ret, "Dhcp success");
            return ret;
        case BS2_EVENT_DEVICE_ADMIN_MENU:
            strcat (ret, "Admin menu");
            return ret;
        case BS2_EVENT_DEVICE_UI_LOCKED:
            strcat (ret, "UI locked");
            return ret;
        case BS2_EVENT_DEVICE_UI_UNLOCKED:
            strcat (ret, "UI unlocked");
            return ret;
        case BS2_EVENT_DEVICE_COMM_LOCKED:
            strcat (ret, "Comm locked");
            return ret;
        case BS2_EVENT_DEVICE_COMM_UNLOCKED:
            strcat (ret, "Comm unlocked");
            return ret;
        case BS2_EVENT_DEVICE_TCP_CONNECTED:
            strcat (ret, "TCP connected");
            if (code == BS2_EVENT_DEVICE_RTSP_CONNECTED) strcpy (ret, "Rtcp connected");
            return ret;
        case BS2_EVENT_DEVICE_TCP_DISCONNECTED:
            strcat (ret, "TCP disconnected");
            if (code == BS2_EVENT_DEVICE_RTSP_DISCONNECTED) strcpy (ret, "Rtcp disconnected");
            return ret;
        case BS2_EVENT_DEVICE_RS485_CONNECTED:
            strcat (ret, "RS485 connected");
            return ret;
        case BS2_EVENT_DEVICE_RS485_DISCONNCTED:
            strcat (ret, "RS485 disconncted");
            return ret;
        case BS2_EVENT_DEVICE_INPUT_DETECTED:
            strcat (ret, "Input detected");
            return ret;
        case BS2_EVENT_DEVICE_TAMPER_ON:
            strcat (ret, "Tamper on");
            return ret;
        case BS2_EVENT_DEVICE_TAMPER_OFF:
            strcat (ret, "Tamper off");
            return ret;
        case BS2_EVENT_DEVICE_EVENT_LOG_CLEARED:
            strcat (ret, "Event log cleared");
            return ret;
        case BS2_EVENT_DEVICE_FIRMWARE_UPGRADED:
            strcat (ret, "Firmware upgraded");
            return ret;
        case BS2_EVENT_DEVICE_RESOURCE_UPGRADED:
            strcat (ret, "Resource upgraded");
            return ret;
        case BS2_EVENT_DEVICE_CONFIG_RESET:
            strcat (ret, "Config reset");

            if (code == BS2_EVENT_DEVICE_DATABASE_RESET) strcpy (ret, "Database reset");
            if (code == BS2_EVENT_DEVICE_FACTORY_RESET) strcpy (ret, "Factory reset");
            if (code == BS2_EVENT_DEVICE_CONFIG_RESET_EX) strcpy (ret, "Config reset ex");
            if (code == BS2_EVENT_DEVICE_FACTORY_RESET_WITHOUT_ETHERNET) strcpy (ret, "Factory reset without ethernet");

            return ret;

        // Supervised Input
        case BS2_EVENT_SUPERVISED_INPUT_SHORT:
            strcat (ret, "Supervised input short");
            return ret;
        case BS2_EVENT_SUPERVISED_INPUT_OPEN:
            strcat (ret, "Supervised input open");
            return ret;

        // Device - Ex
        case BS2_EVENT_DEVICE_AC_FAIL:
            strcat (ret, "Device AC fail");
            return ret;
        case BS2_EVENT_DEVICE_AC_SUCCESS:
            strcat (ret, "Device AC success");
            return ret;

        case BS2_EVENT_EXIT_BUTTON:
            strcat (ret, "Exit button");
            if (code == BS2_EVENT_SIMULATED_EXIT_BUTTON) strcat (ret, " (simulated)");
            return ret;
        case BS2_EVENT_OPERATOR_OPEN:
            strcat (ret, "Operator open");
            return ret;
        case BS2_EVENT_VOIP_OPEN:
            strcat (ret, "VoIP open");
            return ret;


        // Door
        case BS2_EVENT_DOOR_UNLOCKED:
            strcat (ret, "Door unlocked");
            return ret;
        case BS2_EVENT_DOOR_LOCKED:
            strcat (ret, "Door locked");
            return ret;
        case BS2_EVENT_DOOR_OPENED:
            strcat (ret, "Door opened");
            return ret;
        case BS2_EVENT_DOOR_CLOSED:
            strcat (ret, "Door closed");
            return ret;
        case BS2_EVENT_DOOR_FORCED_OPEN:
            strcat (ret, "Door forced open");
            return ret;
        case BS2_EVENT_DOOR_HELD_OPEN:
            strcat (ret, "Door held open");
            return ret;
        case BS2_EVENT_DOOR_FORCED_OPEN_ALARM:
            strcat (ret, "Door forced open alarm");
            return ret;
        case BS2_EVENT_DOOR_FORCED_OPEN_ALARM_CLEAR:
            strcat (ret, "Door forced open alarm clear");
            return ret;
        case BS2_EVENT_DOOR_HELD_OPEN_ALARM:
            strcat (ret, "Door held open alarm");
            return ret;
        case BS2_EVENT_DOOR_HELD_OPEN_ALARM_CLEAR:
            strcat (ret, "Door held open alarm clear");
            return ret;
        case BS2_EVENT_DOOR_APB_ALARM:
            strcat (ret, "Door apb alarm");
            return ret;
        case BS2_EVENT_DOOR_APB_ALARM_CLEAR:
            strcat (ret, "Door apb alarm clear");
            return ret;
        case BS2_EVENT_DOOR_RELEASE:
            strcat (ret, "Door release");
            return ret;
        case BS2_EVENT_DOOR_LOCK:
            strcat (ret, "Door lock");
            return ret;
        case BS2_EVENT_DOOR_UNLOCK:
            strcat (ret, "Door unlock");
            return ret;


        // Zone
        case BS2_EVENT_ZONE_APB_VIOLATION:
        case BS2_EVENT_ZONE_APB_ALARM:
        case BS2_EVENT_ZONE_APB_ALARM_CLEAR:
        case BS2_EVENT_ZONE_TIMED_APB_VIOLATION:
        case BS2_EVENT_ZONE_TIMED_APB_ALARM:
        case BS2_EVENT_ZONE_TIMED_APB_ALARM_CLEAR:
        case BS2_EVENT_ZONE_FIRE_ALARM_INPUT:
        case BS2_EVENT_ZONE_FIRE_ALARM:
        case BS2_EVENT_ZONE_FIRE_ALARM_CLEAR:
        case BS2_EVENT_ZONE_SCHEDULED_LOCK_VIOLATION:
        case BS2_EVENT_ZONE_SCHEDULED_LOCK_START:
        case BS2_EVENT_ZONE_SCHEDULED_LOCK_END:
        case BS2_EVENT_ZONE_SCHEDULED_UNLOCK_START:
        case BS2_EVENT_ZONE_SCHEDULED_UNLOCK_END:
        case BS2_EVENT_ZONE_SCHEDULED_LOCK_ALARM:
        case BS2_EVENT_ZONE_SCHEDULED_LOCK_ALARM_CLEAR:
            strcat (ret, "Zone event");
            return ret;


        // Lift
        case BS2_EVENT_FLOOR_ACTIVATED:
        case BS2_EVENT_FLOOR_DEACTIVATED:
        case BS2_EVENT_FLOOR_RELEASE:
        case BS2_EVENT_FLOOR_ACTIVATE:
        case BS2_EVENT_FLOOR_DEACTIVATE:
        case BS2_EVENT_LIFT_ALARM_INPUT:
        case BS2_EVENT_LIFT_ALARM:
        case BS2_EVENT_LIFT_ALARM_CLEAR:
        case BS2_EVENT_ALL_FLOOR_ACTIVATED:
        case BS2_EVENT_ALL_FLOOR_DEACTIVATED:
            strcat (ret, "User event");
            return ret;


        // BioStar
        case BS2_EVENT_GLOBAL_APB_EXCUSED:
            strcat (ret, "BioStar GlobalAPB excused");
            return ret;


        // TBD
        case BS2_EVENT_DEVICE_ZONE_ENTRANCE_LIMIT_VIOLATION:
        case BS2_EVENT_DEVICE_ZONE_ENTRANCE_LIMIT_ALARM:
        case BS2_EVENT_DEVICE_ZONE_ENTRANCE_LIMIT_ALARM_CLEAR:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_VIOLATION:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_ARM_GRANTED:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_ARM_SUCCESS:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_ARM_FAIL:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_DISARM_GRANTED:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_DISARM_SUCCESS:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_DISARM_FAIL:
        case BS2_EVENT_ZONE_INTRUSION_ALARM:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_CLEAR:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_ARM_DENIED:
        case BS2_EVENT_ZONE_INTRUSION_ALARM_DISARM_DENIED:
        case BS2_EVENT_ZONE_INTERLOCK_VIOLATION:
        case BS2_EVENT_ZONE_INTERLOCK_ALARM:
        case BS2_EVENT_ZONE_INTERLOCK_ALARM_DOOR_OPEN_DENIED:
        case BS2_EVENT_ZONE_INTERLOCK_ALARM_INDOOR_DENIED:
        case BS2_EVENT_ZONE_INTERLOCK_ALARM_CLEAR:
        case BS2_EVENT_ZONE_AUTH_LIMIT_VIOLATION:
        case BS2_EVENT_GLOBAL_AUTH_LIMIT_EXCUSED:
        case BS2_EVENT_ZONE_MUSTER_VIOLATION:
        case BS2_EVENT_ZONE_MUSTER_ALARM:
        case BS2_EVENT_ZONE_MUSTER_ALARM_CLEAR:
        case BS2_EVENT_ZONE_LIFT_LOCK_VIOLATION:
        case BS2_EVENT_ZONE_LIFT_LOCK_START:
        case BS2_EVENT_ZONE_LIFT_LOCK_END:
        case BS2_EVENT_ZONE_LIFT_UNLOCK_START:
        case BS2_EVENT_ZONE_LIFT_UNLOCK_END:
        case BS2_EVENT_ZONE_LIFT_LOCK_ALARM:
        case BS2_EVENT_ZONE_LIFT_LOCK_ALARM_CLEAR:
        case BS2_EVENT_DEVICE_USER_SYNC_TO_SERVER_FAIL:
        case BS2_EVENT_BREAK_GLASS:
        case BS2_EVENT_MEMORY_FULL_MIGRATION:
            strcat (ret, "TBD event");
            return ret;

        // Relay Action (Linkage & Latching)
        case BS2_EVENT_RELAY_ACTION_ON:
            strcat (ret, "Relay ON");
            return ret;
        case BS2_EVENT_RELAY_ACTION_OFF:
            strcat (ret, "Relay OFF");
            return ret;
        case BS2_EVENT_RELAY_ACTION_KEEP:
            strcat (ret, "Relay KEEP");
            return ret;
    }
    return "Unknown";
}

const char * rs485_mode (int mode) {
    switch (mode) {
        case BS2_RS485_MODE_DISABLED: return "Disabled";
        case BS2_RS485_MODE_MASTER: return "Master";
        case BS2_RS485_MODE_SLAVE: return "Slave";
        case BS2_RS485_MODE_STANDALONE: return "Standalone";
        default: break;
    }
    return "Unknown";
}

const char * door_flag (int flags) {
    static char buf[512];
    memset (buf, 0, 512);
    if ((flags & BS2_DOOR_FLAG_ALL) == BS2_DOOR_FLAG_ALL) {
        strcat (buf, "All");
    } else {
        if (flags & BS2_DOOR_FLAG_SCHEDULE) strcat (buf, "Schedule ");
        if (flags & BS2_DOOR_FLAG_OPERATOR) strcat (buf, "Operator ");
        if (flags & BS2_DOOR_FLAG_EMERGENCY) strcat (buf, "Emergency ");
    }
    if (buf[0] == 0) {
        strcat (buf, "None");
    }
    return buf;
}

const char * format_date (time_t time) {
    static char date[32] = {0};
    struct tm * t = localtime (&time);
    strftime (date, 31, "%F %T", t);
    return date;
}

const char * join_uint32_array (uint32_t *arr, const char *del, uint32_t count) {
    static char buf[2048];
    char dig[32] = {0};
    uint32_t i;

    memset (buf, 0, 2048);

    for (i = 0; i < count; i++) {
        if (arr[i]) {
            if (i) strcat (buf, del);
            sprintf (dig, "%u", arr[i]);
            strcat (buf, dig);
        }
    }

    return buf;
}

const char * card_data (uint8_t *data) {
    static char buf[128];
    char dig[4] = {0};
    bool hasVal = false;
    int i;

    memset (buf, 0, 128);

    for (i=0; i < BS2_CARD_DATA_SIZE; i++) {
        if (data[i]) {
            sprintf (dig, "%02X", data[i]);
            strcat (buf, dig);
            hasVal = true;
        } else {
            if (hasVal) {
                strcat (buf, "00");
            }
        }
    }

    return buf;
}