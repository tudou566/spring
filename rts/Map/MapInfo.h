/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef MAP_INFO_H
#define MAP_INFO_H

#include "System/float3.h"
#include "System/float4.h"

#include <string>
#include <vector>

class LuaTable;
class MapParser;
#if !defined(HEADLESS) && !defined(NO_SOUND)
	struct EAXSfxProps;
#endif


class CMapInfo
{
public:
	/** Terrain type, there can be 256 of these:
	    "MAP\TerrainType0" up to "MAP\TerrainType255" */
	static const int NUM_TERRAIN_TYPES = 256;

	/**
	 * @param mapInfoFile mapinfo file, aka sm3 / smf (full path)
	 * @param mapName human readable mapname e.g. DeltaSiegeDry
	 */
	CMapInfo(const std::string& mapInfoFile, const std::string& mapName);
	~CMapInfo();

	/* The settings are just public members because:

	   1) it's quite some work to encapsulate all of them, and
	   2) nothing too bad happens if you modify them, there are no complex
	      pointer members that really beg for encapsulation.

	   Instead of encapsulation it is as effective and much easier make the
	   global mapInfo const, ie. const CMapInfo* mapInfo;
	 */

	/* Note: this could (should) have been anonymous structures if only MSVC 8
	   didn't crap out on it.  Specifically, it craps out on any class with at
	   least 1 user defined non-inline constructor and at least 2 anonymous
	   structures, each with at least 1 object with a constructor in it.
	   In other words, it probably assigns the same name to each anonymous
	   structure, and later gets confused by that.

	   This sample code triggers the problem in MSVC:

		class A {
			A::A();
			struct { std::string s1; } a1;
			struct { std::string s2; } a2;
		};
		A::A() {}
	 */

	std::string GetStringValue(const std::string& key) const; // can be used before Load()

	/** Global settings, ie. from "MAP" section. */
	struct map_t {
		std::string name;        ///< The filename as passed to the constructor.
		std::string description; ///< "MAP\\Description"
		std::string author;
		float hardness;          ///< "MAP\\MapHardness"
		bool  notDeformable;
		/** Stores the gravity as a negative number in units/frame^2
		    (NOT positive units/second^2 as in the mapfile) */
		float gravity;
		float tidalStrength;
		float maxMetal;        ///< what metal value 255 in the metal map is worth
		float extractorRadius; ///< extraction radius for mines
		float voidAlphaMin;
		bool  voidWater;
		bool  voidGround;
	} map;

	/** GUI settings (used by CGuiHandler) */
	struct gui_t {
		bool autoShowMetal;
	} gui;

	/** settings read from "MAP\ATMOSPHERE" section */
	struct atmosphere_t {
		float  fluidDensity; ///< in kg/m^3
		float  cloudDensity;
		float  fogStart;
		float  fogEnd;
		float4 fogColor;
		float3 skyColor;
		float3 skyDir;
		float3 sunColor;
		float3 cloudColor;
		float  minWind;
		float  maxWind;
		std::string skyBox;
	} atmosphere;

	/** settings read from "MAP\SPLATS" section */
	struct splats_t {
		float4 texScales;
		float4 texMults;
	} splats;

	/** settings read from "MAP\GRASS" section */
	struct grass_t {
		float bladeWaveScale; //! how strongly wind affects grass-blade waving (if 0, disables vertex animation)
		float bladeWidth;
		float bladeHeight;    //! actual blades will be (bladeHeight + randf(0, bladeHeight)) tall
		float bladeAngle;
		int maxStrawsPerTurf;
		float3 color;
		std::string bladeTexName;    // defaults to internally-generated texture
	} grass;

	/** settings read from "MAP\LIGHT" section */
	struct light_t {
		float4 sunDir;     ///< Holds vector for the direction of the sun
		float sunOrbitTime;
		float sunStartAngle;
		float3 groundAmbientColor;
		float3 groundSunColor;
		float3 groundSpecularColor;
		float  groundShadowDensity;
		float4 unitAmbientColor;
		float4 unitSunColor;
		float  unitShadowDensity;
		float3 unitSpecularColor;
		float  specularExponent;
	} light;

	/** settings read from "MAP\WATER" section
	    prefix their name with "Water" to get the TDF variable */
	struct water_t {
		float  fluidDensity;      ///< in kg/m^3
		float  repeatX;           ///< (calculated default is in IWater)
		float  repeatY;           ///< (calculated default is in IWater)
		float  damage;            ///< scaled by (UNIT_SLOWUPDATE_RATE / GAME_SPEED)
		float3 absorb;
		float3 baseColor;
		float3 minColor;
		float3 surfaceColor;
		float  surfaceAlpha;
		float4 planeColor;
		float3 diffuseColor;
		float3 specularColor;
		float  ambientFactor;
		float  diffuseFactor;
		float  specularFactor;
		float  specularPower;
		float  fresnelMin;
		float  fresnelMax;
		float  fresnelPower;
		float  reflDistortion;
		float  blurBase;
		float  blurExponent;
		float  perlinStartFreq;
		float  perlinLacunarity;
		float  perlinAmplitude;
		float  windSpeed;
		bool   shoreWaves;
		bool   forceRendering;    ///< if false the renderers will render it only if currentMinMapHeight<0
		bool   hasWaterPlane;     ///< true if "MAP\WATER\WaterPlaneColor" is set
		unsigned char numTiles;
		std::string texture;
		std::string foamTexture;
		std::string normalTexture;
		std::vector<std::string> causticTextures;
	} water;

	/** SMF specific settings */
	struct smf_t {
		std::string detailTexName;        ///< "MAP\DetailTex"
		std::string specularTexName;      ///< "MAP\SpecularTex"
		std::string splatDistrTexName;
		std::string splatDetailTexName;
		std::string grassShadingTexName;  // defaults to minimap texture
		std::string skyReflectModTexName;
		std::string detailNormalTexName;
		std::string lightEmissionTexName;
		std::string parallaxHeightTexName;

		std::string splatDetailNormalTex1Name; //Contains the splatted detail normal textures 1-4
		std::string splatDetailNormalTex2Name;
		std::string splatDetailNormalTex3Name;
		std::string splatDetailNormalTex4Name;
		//Controls wether the alpha channel of each splatted detail normal texture 
		//contains a diffuse channel, which behaves like the old splatted detail textures
		bool  splatDetailNormalDiffuseAlpha; 
		// SMF overrides
		std::string minimapTexName;
		std::string typemapTexName;
		std::string metalmapTexName;
		std::string grassmapTexName;


		float minHeight;
		bool  minHeightOverride;
		float maxHeight;
		bool  maxHeightOverride;

		std::vector<std::string> smtFileNames;
	} smf;

	/** SM3 specific settings
	    This is NOT complete, SM3 stores so much in the map settings
	    that it really isn't a good idea to put them here. */
	struct sm3_t {
		std::string minimap; ///< "MAP\minimap"
	} sm3;

	struct pfs_t {
		struct legacy_constants_t {
		} legacy_constants;

		struct qtpfs_constants_t {
			unsigned int layersPerUpdate;
			unsigned int maxTeamSearches;
			unsigned int minNodeSizeX;
			unsigned int minNodeSizeZ;
			unsigned int maxNodeDepth;
			unsigned int numSpeedModBins;
			float        minSpeedModVal;
			float        maxSpeedModVal;
		} qtpfs_constants;
	} pfs;


	struct TerrainType {
		std::string name;
		float hardness;
		float tankSpeed;   ///< "TankMoveSpeed"
		float kbotSpeed;   ///< "KbotMoveSpeed"
		float hoverSpeed;  ///< "HoverMoveSpeed"
		float shipSpeed;   ///< "ShipMoveSpeed"
		bool receiveTracks;
	};

	TerrainType terrainTypes[NUM_TERRAIN_TYPES];

	/**
	 * Sound EFX param structure
	 */
#if !defined(HEADLESS) && !defined(NO_SOUND)
	EAXSfxProps* efxprops;
#endif

private:
	void ReadGlobal();
	void ReadGui();
	void ReadAtmosphere();
	void ReadSplats();
	void ReadGrass();
	void ReadLight();
	void ReadWater();
	void ReadSMF();
	void ReadSM3();
	void ReadTerrainTypes();
	void ReadPFSConstants();
	void ReadSound();

	MapParser* parser; // map       parser root table
	LuaTable* resRoot; // resources parser root table
};

extern const CMapInfo* mapInfo;

#endif // MAP_INFO_H
