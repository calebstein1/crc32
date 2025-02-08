#include <arm_acle.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char *argv[]) {
	int f;
	uint32_t crc32c;
	size_t b, i, j, w;
	uint8_t buff[0x20000];
	uint64_t *buff_p64 = (uint64_t *)buff;
	uint8_t *buff_p8;

	if (argc < 2) {
		puts("Must supply at least one file!");
		return 1;
	}

	for (i = 1; i < argc; i++) {
		crc32c = 0xffffffff;
		f = open(argv[i], O_RDONLY);
		if (f == -1) {
			printf("Could not locate file: %s\n", argv[i]);
			continue;
		}

		while((b = read(f, buff, sizeof(buff))) > 0) {
			w = b / 8;
			for (j = 0; j < w; j++) {
				crc32c = __crc32cd(crc32c, *buff_p64++);
			}
			buff_p8 = (uint8_t *)buff_p64;
			for (j = w * 8; j < b; j++) {
				crc32c = __crc32cb(crc32c, *buff_p8++);
			}
			buff_p64 = (uint64_t *)buff;
		}
		printf("%s: %u\n", argv[i], crc32c ^ 0xffffffff);
		close(f);
	}
	return 0;
}
