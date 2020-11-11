#include <pskc/pskc.h>
#include <stdio.h>
#include <string.h>

static const char *testkey = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<KeyContainer xmlns=\"urn:ietf:params:xml:ns:keyprov:pskc\" "
               "Version=\"1.0\" Id=\"exampleID1\">"
  "<KeyPackage><Key Id=\"12345678\" "
                   "Algorithm=\"urn:ietf:params:xml:ns:keyprov:pskc:hotp\">"
      "<Data>"
	"<Secret>"
          "<PlainValue>YoTOTvAMztz6JhLQGaoUqhS1f/C0+Jr0B7xIfA0=</PlainValue>"
        "</Secret>"
        "<Counter>"
          "<PlainValue>35</PlainValue>"
        "</Counter>"
      "</Data>"
   "</Key>"
  "</KeyPackage>"
"</KeyContainer>";

int main(void)
{
	pskc_t *container;
	pskc_key_t *key;
	char *buf;
	size_t s;

	if (pskc_global_init() || pskc_init(&container) ||
	    pskc_parse_from_memory(container, strlen(testkey), testkey) ||
	    !(key = pskc_get_keypackage(container, 0))) {
		fprintf(stderr, "init error\n");
		return 1;
	}
	printf("Counter: %d\n", pskc_get_key_data_counter(key, NULL));

	pskc_set_key_data_counter(key, pskc_get_key_data_counter(key, NULL) + 1);
	pskc_output(container, PSKC_OUTPUT_XML, &buf, &s);
	printf("output: %.*s\n", s, buf);
	pskc_build_xml(container, &buf, &s);
	printf("New XML: '%.*s'\n", s, buf);
	pskc_build_xml(container, &buf, &s);
	printf("XML again: '%.*s'\n", s, buf);
	return 0;
}

