const char mainPage[] PROGMEM = R"=====(
<HTML>
    <HEAD>
        <TITLE>My first web page</TITLE>
    </HEAD>
    <BODY>
        <CENTER>
            <B>Hello World.... </B>
            <a href="https://accounts.spotify.com/authorize?response_type=code&client_id=%s&redirect_uri=%s&scope=user-modify-playback-state user-read-currently-playing user-read-playback-state user-library-modify user-library-read">Log in to spotify</a>
        </CENTER>
    </BODY>
</HTML>
)=====";

const char errorPage[] PROGMEM = R"=====(
<HTML>
    <HEAD>
        <TITLE>My first web page</TITLE>
    </HEAD>
    <BODY>
        <CENTER>
            <B>Hello World.... </B>
            <a href="https://accounts.spotify.com/authorize?response_type=code&client_id=%s&redirect_uri=%s&scope=user-modify-playback-state user-read-currently-playing user-read-playback-state user-library-modify user-library-read">Log in to spotify</a>
        </CENTER>
    </BODY>
</HTML>
)=====";