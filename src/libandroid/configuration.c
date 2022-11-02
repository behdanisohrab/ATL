
typedef void AConfiguration;
typedef void AAssetManager;

void AConfiguration_getCountry(AConfiguration* config, char* outCountry) {
	/* Assume not set. */
	outCountry[0] = 0;
	outCountry[1] = 0;
}

void AConfiguration_getLanguage(AConfiguration* config, char* outLanguage) {
	/* Assume not set. */
	outLanguage[0] = 0;
	outLanguage[1] = 0;
}

void AConfiguration_delete(AConfiguration *config) {}

void * AConfiguration_new(void) { return 0; }

void AConfiguration_fromAssetManager(AConfiguration *out, AAssetManager *am)
{
}
