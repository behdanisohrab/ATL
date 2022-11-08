#include <stdint.h>

typedef void AConfiguration;
typedef void AAssetManager;

/**
 * Create a new AConfiguration, initialized with no values set.
 */
AConfiguration* AConfiguration_new()
{
	return 0;
}

/**
 * Free an AConfiguration that was previously created with
 * AConfiguration_new().
 */
void AConfiguration_delete(AConfiguration* config)
{
	return;
}

/**
 * Create and return a new AConfiguration based on the current configuration in
 * use in the given {@link AAssetManager}.
 */
void AConfiguration_fromAssetManager(AConfiguration* out, AAssetManager* am)
{
	return;
}

/**
 * Copy the contents of 'src' to 'dest'.
 */
void AConfiguration_copy(AConfiguration* dest, AConfiguration* src)
{
	return;
}

/**
 * Return the current MCC set in the configuration.  0 if not set.
 */
int32_t AConfiguration_getMcc(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current MCC in the configuration.  0 to clear.
 */
void AConfiguration_setMcc(AConfiguration* config, int32_t mcc)
{
	return;
}

/**
 * Return the current MNC set in the configuration.  0 if not set.
 */
int32_t AConfiguration_getMnc(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current MNC in the configuration.  0 to clear.
 */
void AConfiguration_setMnc(AConfiguration* config, int32_t mnc)
{
	return;
}

/**
 * Return the current language code set in the configuration.  The output will
 * be filled with an array of two characters.  They are not 0-terminated.  If
 * a language is not set, they will be 0.
 */
void AConfiguration_getLanguage(AConfiguration* config, char* outLanguage) {
	/* Assume not set. */
	outLanguage[0] = 0;
	outLanguage[1] = 0;
}

/**
 * Set the current language code in the configuration, from the first two
 * characters in the string.
 */
void AConfiguration_setLanguage(AConfiguration* config, const char* language)
{
	return;
}

/**
 * Return the current country code set in the configuration.  The output will
 * be filled with an array of two characters.  They are not 0-terminated.  If
 * a country is not set, they will be 0.
 */
void AConfiguration_getCountry(AConfiguration* config, char* outCountry) {
	/* Assume not set. */
	outCountry[0] = 0;
	outCountry[1] = 0;
}

/**
 * Set the current country code in the configuration, from the first two
 * characters in the string.
 */
void AConfiguration_setCountry(AConfiguration* config, const char* country)
{
	return;
}

/**
 * Return the current ACONFIGURATION_ORIENTATION_* set in the configuration.
 */
int32_t AConfiguration_getOrientation(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current orientation in the configuration.
 */
void AConfiguration_setOrientation(AConfiguration* config, int32_t orientation)
{
	return;
}

/**
 * Return the current ACONFIGURATION_TOUCHSCREEN_* set in the configuration.
 */
int32_t AConfiguration_getTouchscreen(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current touchscreen in the configuration.
 */
void AConfiguration_setTouchscreen(AConfiguration* config, int32_t touchscreen)
{
	return;
}

/**
 * Return the current ACONFIGURATION_DENSITY_* set in the configuration.
 */
int32_t AConfiguration_getDensity(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current density in the configuration.
 */
void AConfiguration_setDensity(AConfiguration* config, int32_t density)
{
	return;
}

/**
 * Return the current ACONFIGURATION_KEYBOARD_* set in the configuration.
 */
int32_t AConfiguration_getKeyboard(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current keyboard in the configuration.
 */
void AConfiguration_setKeyboard(AConfiguration* config, int32_t keyboard)
{
	return;
}

/**
 * Return the current ACONFIGURATION_NAVIGATION_* set in the configuration.
 */
int32_t AConfiguration_getNavigation(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current navigation in the configuration.
 */
void AConfiguration_setNavigation(AConfiguration* config, int32_t navigation)
{
	return;
}

/**
 * Return the current ACONFIGURATION_KEYSHIDDEN_* set in the configuration.
 */
int32_t AConfiguration_getKeysHidden(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current keys hidden in the configuration.
 */
void AConfiguration_setKeysHidden(AConfiguration* config, int32_t keysHidden)
{
	return;
}

/**
 * Return the current ACONFIGURATION_NAVHIDDEN_* set in the configuration.
 */
int32_t AConfiguration_getNavHidden(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current nav hidden in the configuration.
 */
void AConfiguration_setNavHidden(AConfiguration* config, int32_t navHidden)
{
	return;
}

/**
 * Return the current SDK (API) version set in the configuration.
 */
int32_t AConfiguration_getSdkVersion(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current SDK version in the configuration.
 */
void AConfiguration_setSdkVersion(AConfiguration* config, int32_t sdkVersion)
{
	return;
}

/**
 * Return the current ACONFIGURATION_SCREENSIZE_* set in the configuration.
 */
int32_t AConfiguration_getScreenSize(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current screen size in the configuration.
 */
void AConfiguration_setScreenSize(AConfiguration* config, int32_t screenSize)
{
	return;
}

/**
 * Return the current ACONFIGURATION_SCREENLONG_* set in the configuration.
 */
int32_t AConfiguration_getScreenLong(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current screen long in the configuration.
 */
void AConfiguration_setScreenLong(AConfiguration* config, int32_t screenLong)
{
	return;
}

/**
 * Return the current ACONFIGURATION_SCREENROUND_* set in the configuration.
 */
int32_t AConfiguration_getScreenRound(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current screen round in the configuration.
 */
void AConfiguration_setScreenRound(AConfiguration* config, int32_t screenRound)
{
	return;
}

/**
 * Return the current ACONFIGURATION_UI_MODE_TYPE_* set in the configuration.
 */
int32_t AConfiguration_getUiModeType(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current UI mode type in the configuration.
 */
void AConfiguration_setUiModeType(AConfiguration* config, int32_t uiModeType)
{
	return;
}

/**
 * Return the current ACONFIGURATION_UI_MODE_NIGHT_* set in the configuration.
 */
int32_t AConfiguration_getUiModeNight(AConfiguration* config)
{
	return -1;
}

/**
 * Set the current UI mode night in the configuration.
 */
void AConfiguration_setUiModeNight(AConfiguration* config, int32_t uiModeNight)
{
	return;
}

#if __ANDROID_API__ >= 13
/**
 * Return the current configuration screen width in dp units, or
 * ACONFIGURATION_SCREEN_WIDTH_DP_ANY if not set.
 */
int32_t AConfiguration_getScreenWidthDp(AConfiguration* config)
{
	return -1;
}

/**
 * Set the configuration's current screen width in dp units.
 */
void AConfiguration_setScreenWidthDp(AConfiguration* config, int32_t value)
{
	return;
}

/**
 * Return the current configuration screen height in dp units, or
 * ACONFIGURATION_SCREEN_HEIGHT_DP_ANY if not set.
 */
int32_t AConfiguration_getScreenHeightDp(AConfiguration* config)
{
	return -1;
}

/**
 * Set the configuration's current screen width in dp units.
 */
void AConfiguration_setScreenHeightDp(AConfiguration* config, int32_t value)
{
	return;
}

/**
 * Return the configuration's smallest screen width in dp units, or
 * ACONFIGURATION_SMALLEST_SCREEN_WIDTH_DP_ANY if not set.
 */
int32_t AConfiguration_getSmallestScreenWidthDp(AConfiguration* config)
{
	return -1;
}

/**
 * Set the configuration's smallest screen width in dp units.
 */
void AConfiguration_setSmallestScreenWidthDp(AConfiguration* config, int32_t value)
{
	return;
}
#endif /* __ANDROID_API__ >= 13 */

#if __ANDROID_API__ >= 17
/**
 * Return the configuration's layout direction, or
 * ACONFIGURATION_LAYOUTDIR_ANY if not set.
 */
int32_t AConfiguration_getLayoutDirection(AConfiguration* config)
{
	return -1;
}

/**
 * Set the configuration's layout direction.
 */
void AConfiguration_setLayoutDirection(AConfiguration* config, int32_t value)
{
	return;
}
#endif /* __ANDROID_API__ >= 17 */

/**
 * Perform a diff between two configurations.  Returns a bit mask of
 * ACONFIGURATION_* constants, each bit set meaning that configuration element
 * is different between them.
 */
int32_t AConfiguration_diff(AConfiguration* config1, AConfiguration* config2)
{
	return -1;
}

/**
 * Determine whether 'base' is a valid configuration for use within the
 * environment 'requested'.  Returns 0 if there are any values in 'base'
 * that conflict with 'requested'.  Returns 1 if it does not conflict.
 */
int32_t AConfiguration_match(AConfiguration* base, AConfiguration* requested)
{
	return -1;
}

/**
 * Determine whether the configuration in 'test' is better than the existing
 * configuration in 'base'.  If 'requested' is non-NULL, this decision is based
 * on the overall configuration given there.  If it is NULL, this decision is
 * simply based on which configuration is more specific.  Returns non-0 if
 * 'test' is better than 'base'.
 *
 * This assumes you have already filtered the configurations with
 * AConfiguration_match().
 */
int32_t AConfiguration_isBetterThan(AConfiguration* base, AConfiguration* test,
        AConfiguration* requested);
