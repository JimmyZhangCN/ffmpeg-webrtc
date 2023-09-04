#include <stdio.h>
#include <stdint.h>
#include <curl/curl.h>

#include <yangutil/sys/YangHttpsSocket.h>
#include <yangutil/sys/YangLog.h>

#include <string.h>

// [09:40:50] Yang ERROR: post buf:POST /rtc/v1/play/ HTTP/1.1
// Host: 127.0.0.1:1985
// Accept: */*

// Content-Type:application/json;charset=UTF-8
// Content-Length: 1817

// {
//   "api": "http://127.0.0.1:1985/rtc/v1/play/",
//   "streamurl": "webrtc://127.0.0.1:1985/live/livestream",
//   "clientip": null,
//   "sdp": "v=0\r\no=MetaRtc 140469688915736 2 IN IP4 0.0.0.0\r\ns=MetaRtcSession\r\nt=0 0\r\na=ice-lite\r\na=group:BUNDLE 0 1\r\na=msid-semantic: WMS live/livestream\r\nm=audio 9 UDP/TLS/RTP/SAVPF 111\r\nc=IN IP4 0.0.0.0\r\na=ice-ufrag:6h18\r\na=ice-pwd:01122kub723862j0o35317v2n23994nj\r\na=fingerprint:sha-256 AD:8B:BB:10:A9:22:9D:09:70:95:5A:12:50:C3:63:08:43:F2:2A:C1:B8:0E:2F:AA:25:8D:45:1C:F3:7E:07:3E\r\na=setup:actpass\r\na=mid:0\r\na=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=recvonly\r\na=rtcp-mux\r\na=rtcp-rsize\r\na=rtpmap:111 opus/48000/2\r\na=rtcp-fb:111 transport-cc\r\na=fmtp:111 minptime=10;useinbandfec=1\r\na=ssrc:-540447691 cname:v41445zv483ub24f\r\na=ssrc:-540447691 msid:- 568xt09g-w295-7248-6p24-1izz089r0476\r\na=ssrc:-540447691 mslabel:-\r\na=ssrc:-540447691 label:568xt09g-w295-7248-6p24-1izz089r0476\r\nm=video 9 UDP/TLS/RTP/SAVPF 125 114\r\nc=IN IP4 0.0.0.0\r\na=ice-ufrag:6h18\r\na=ice-pwd:01122kub723862j0o35317v2n23994nj\r\na=fingerprint:sha-256 AD:8B:BB:10:A9:22:9D:09:70:95:5A:12:50:C3:63:08:43:F2:2A:C1:B8:0E:2F:AA:25:8D:45:1C:F3:7E:07:3E\r\na=setup:actpass\r\na=mid:1\r\na=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=recvonly\r\na=rtcp-mux\r\na=rtcp-rsize\r\na=rtpmap:125 H264/90000\r\na=rtcp-fb:125 transport-cc\r\na=rtcp-fb:125 nack\r\na=rtcp-fb:125 nack pli\r\na=fmtp:125 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f\r\na=rtpmap:114 red/90000\r\na=ssrc:-540447690 cname:v41445zv483ub24f\r\na=ssrc:-540447690 msid:- a5335297-vo62-0066-63q1-l56l35d3f095\r\na=ssrc:-540447690 mslabel:-\r\na=ssrc:-540447690 label:a5335297-vo62-0066-63q1-l56l35d3f095\r\n"
// }

// [09:40:50] Yang ERROR: sdp rets:HTTP/1.1 200 OK
// Connection: Close
// Content-Length: 1086
// Content-Type: application/json
// Server: SRS/5.0.146(Bee)

// {
//   "code": 0,
//   "server": "vid-6p3v224",
//   "service": "043k6y41",
//   "pid": "299512",
//   "sdp": "v=0\r\no=SRS/5.0.146(Bee) 107545984450688 2 IN IP4 0.0.0.0\r\ns=SRSPlaySession\r\nt=0 0\r\na=ice-lite\r\na=group:BUNDLE 0\r\na=msid-semantic: WMS live/livestream\r\nm=audio 9 UDP/TLS/RTP/SAVPF 63 111\r\nc=IN IP4 0.0.0.0\r\na=ice-ufrag:075511e0\r\na=ice-pwd:1056yaf9z2509j259322o519z05674b9\r\na=fingerprint:sha-256 2D:01:D8:7C:45:C1:1B:41:C1:22:9B:ED:A0:B7:B3:54:1C:58:EE:CF:20:9B:C7:FD:C0:69:A4:92:8C:17:93:86\r\na=setup:passive\r\na=mid:0\r\na=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\na=sendonly\r\na=rtcp-mux\r\na=rtcp-rsize\r\na=rtpmap:63 red/48000/2\r\na=rtpmap:111 opus/48000/2\r\na=rtcp-fb:111 transport-cc\r\na=fmtp:111 minptime=10;useinbandfec=1\r\na=ssrc:3025370772 cname:8661x3cgg1cml7fl\r\na=ssrc:3025370772 msid:- aaf25f76-35d0-4e42-b875-74dc4b0d645d\r\na=ssrc:3025370772 mslabel:-\r\na=ssrc:3025370772 label:aaf25f76-35d0-4e42-b875-74dc4b0d645d\r\na=candidate:0 1 udp 2130706431 192.168.0.8 8000 typ host generation 0\r\n",
//   "sessionid": "075511e0:6h18"
// }
typedef struct data_packet_
{
	char ptr[4096];
	long total;
	long position;
} data_packet;

static void dump(const char *text,
				 FILE *stream, unsigned char *ptr, size_t size)
{
	size_t i;
	size_t c;
	unsigned int width = 0x10;

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
			text, (long)size, (long)size);

	for (i = 0; i < size; i += width)
	{
		fprintf(stream, "%4.4lx: ", (long)i);

		/* show hex to the left */
		for (c = 0; c < width; c++)
		{
			if (i + c < size)
				fprintf(stream, "%02x ", ptr[i + c]);
			else
				fputs("   ", stream);
		}

		/* show data on the right */
		for (c = 0; (c < width) && (i + c < size); c++)
		{
			char x = (ptr[i + c] >= 0x20 && ptr[i + c] < 0x80) ? ptr[i + c] : '.';
			fputc(x, stream);
		}

		fputc('\n', stream); /* newline */
	}
}

static int my_trace(CURL *handle, curl_infotype type,
					char *data, size_t size,
					void *clientp)
{
	const char *text;
	(void)handle; /* prevent compiler warning */
	(void)clientp;

	switch (type)
	{
	case CURLINFO_TEXT:
		fputs("== Info: ", stderr);
		fwrite(data, size, 1, stderr);
	default: /* in case a new one is introduced to shock us */
		return 0;

	case CURLINFO_HEADER_OUT:
		text = "=> Send header";
		break;
	case CURLINFO_DATA_OUT:
		text = "=> Send data";
		break;
	case CURLINFO_SSL_DATA_OUT:
		text = "=> Send SSL data";
		break;
	case CURLINFO_HEADER_IN:
		text = "<= Recv header";
		break;
	case CURLINFO_DATA_IN:
		text = "<= Recv data";
		break;
	case CURLINFO_SSL_DATA_IN:
		text = "<= Recv SSL data";
		break;
	}

	dump(text, stderr, (unsigned char *)data, size);
	return 0;
}

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userp)
{
	data_packet *data = (data_packet *)userp;
	if (data == NULL || size * nmemb < 1)
	{
		return 0;
	}
	// fprintf(stdout, "read size:%ld nmemb:%ld", size, nmemb); // size:1 nmemb:65524
	if (data->total > data->position)
	{
		size_t wangted_len = size * nmemb;
		size_t left_len = data->total - data->position;
		size_t real_len = left_len >= wangted_len ? wangted_len : left_len;
		memcpy(ptr, &data->ptr[data->position], real_len);
		data->position += real_len;
		return real_len;
	}
	else
	{
		return 0;
	}
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userp)
{
	data_packet *data = (data_packet *)userp;
	if (data == NULL || size * nmemb < 1)
	{
		return 0;
	}

	memcpy((void *)&data->ptr[data->position], ptr, size * nmemb);
	data->position += size * nmemb;
	data->total = data->position;
	return size * nmemb;
}

int32_t yang_https_post(char *rets, char *ip, int32_t port, char *api,
						uint8_t *data, int32_t plen)
{
	int32_t ret = 1;
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: */*");
		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");

		char dest_url[128] = {0};
		snprintf(dest_url, 128, "https://%s:%d/%s", ip, port, api);
		curl_easy_setopt(curl, CURLOPT_URL, dest_url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		fprintf(stdout, "url %s\nsend sdp:%s\n", dest_url, data);

		data_packet recved_data;
		memset((void *)&recved_data, 0, sizeof(recved_data));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recved_data);

		data_packet send_data;
		memset((void *)&send_data, 0, sizeof(send_data));
		memcpy((void *)send_data.ptr, data, plen);
		send_data.total = plen;
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &send_data);
		/*
		 * If you want to connect to a site who is not using a certificate that is
		 * signed by one of the certs in the CA bundle you have, you can skip the
		 * verification of the server's certificate. This makes the connection
		 * A LOT LESS SECURE.
		 *
		 * If you have a CA cert for the server stored someplace else than in the
		 * default bundle, then the CURLOPT_CAPATH option might come handy for
		 * you.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		/*
		 * If the site you are connecting to uses a different host name that what
		 * they have mentioned in their server certificate's commonName (or
		 * subjectAltName) fields, libcurl will refuse to connect. You can skip
		 * this check, but this will make the connection less secure.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		// curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		}
		else
		{
			memcpy(rets, recved_data.ptr, recved_data.position);
			fprintf(stdout, "result sdp:%s\n", rets);
			ret = 0;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return ret;
}
