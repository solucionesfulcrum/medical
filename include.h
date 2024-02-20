#ifndef INCLUDE
#define INCLUDE


#define study_created   "0"
#define study_finished  "1"
#define study_sended    "2"

#define state_incomplete        "-1"
#define state_ontransmission    "0"
#define state_send              "1"
#define state_assigned          "2"
#define state_diagnosticated    "3"

#define encrypt_database        1

#include <QString>
#include <QtWidgets>
#include <QMainWindow>

static QString uncompressedvideoname = "uncompressedvideo.yuv";
static QString compressedvideoname = "compressedvideo.mp4";
static QString metafilename = "meta";

static QString cryptedcompressedvideoname = "cryptedcompressedvideo.mp4";
static QString cryptedmetafilename = "cryptedmeta";
//Tochange
static QString defaultffmpeguncompressed  = "ffmpeg -i 17-54-02.avi -f rawvideo -vcodec rawvideo -pix_fmt yuv420p -s 864x768 -r 10 rawvideo.yuv";
static QString apikey  = "$2gyf$10a$nRLwp4Ea2R9MMoIqJ4ateloceT6ua59LMuC5WLlQamGKT5PU8GVcXfDukO";

static QString check  = "medicalEco25684po1455";

static QString apiurlprotocols      = "/api/protocols";
static QString apiurlsend           = "/api/dicoms/save";
static QString apiurlcheckstudy     = "/api/studies/status";
static QString apiurlstudyConfirm   = "/api/studies/confirm";
static QString apiurlcheckConn      = "/api/serverStatus";


#endif // INCLUDE

