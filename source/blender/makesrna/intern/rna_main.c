/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file blender/makesrna/intern/rna_main.c
 *  \ingroup RNA
 */

#include <stdlib.h>
#include <string.h>

#include "BLI_utildefines.h"
#include "BLI_path_util.h"

#include "RNA_define.h"
#include "RNA_access.h"

#include "rna_internal.h"

#ifdef RNA_RUNTIME

#include "BKE_main.h"
#include "BKE_mesh.h"
#include "BKE_global.h"

/* all the list begin functions are added manually here, Main is not in SDNA */

static bool rna_Main_use_autopack_get(PointerRNA *UNUSED(ptr))
{
	if (G.fileflags & G_AUTOPACK)
		return 1;

	return 0;
}

static void rna_Main_use_autopack_set(PointerRNA *UNUSED(ptr), bool value)
{
	if (value)
		G.fileflags |= G_AUTOPACK;
	else
		G.fileflags &= ~G_AUTOPACK;
}

static bool rna_Main_is_saved_get(PointerRNA *UNUSED(ptr))
{
	return G.relbase_valid;
}

static bool rna_Main_is_dirty_get(PointerRNA *ptr)
{
	/* XXX, not totally nice to do it this way, should store in main ? */
	Main *bmain = (Main *)ptr->data;
	wmWindowManager *wm;
	if ((wm = bmain->wm.first)) {
		return !wm->file_saved;
	}

	return true;
}

static void rna_Main_filepath_get(PointerRNA *ptr, char *value)
{
	Main *bmain = (Main *)ptr->data;
	BLI_strncpy(value, bmain->name, sizeof(bmain->name));
}

static int rna_Main_filepath_length(PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	return strlen(bmain->name);
}

#if 0
static void rna_Main_filepath_set(PointerRNA *ptr, const char *value)
{
	Main *bmain = (Main *)ptr->data;
	BLI_strncpy(bmain->name, value, sizeof(bmain->name));
}
#endif

static void rna_Main_scene_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->scene, NULL);
}

static void rna_Main_object_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->object, NULL);
}

static void rna_Main_light_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->lamp, NULL);
}

static void rna_Main_library_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->library, NULL);
}

static void rna_Main_mesh_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->mesh, NULL);
}

static void rna_Main_curve_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->curve, NULL);
}

static void rna_Main_mball_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->mball, NULL);
}

static void rna_Main_mat_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->mat, NULL);
}

static void rna_Main_tex_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->tex, NULL);
}

static void rna_Main_image_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->image, NULL);
}

static void rna_Main_latt_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->latt, NULL);
}

static void rna_Main_camera_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->camera, NULL);
}

static void rna_Main_key_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->key, NULL);
}

static void rna_Main_world_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->world, NULL);
}

static void rna_Main_screen_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->screen, NULL);
}

static void rna_Main_font_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->vfont, NULL);
}

static void rna_Main_text_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->text, NULL);
}

static void rna_Main_speaker_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->speaker, NULL);
}

static void rna_Main_sound_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->sound, NULL);
}

static void rna_Main_collection_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->collection, NULL);
}

static void rna_Main_armature_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->armature, NULL);
}

static void rna_Main_action_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->action, NULL);
}

static void rna_Main_nodetree_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->nodetree, NULL);
}

static void rna_Main_brush_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->brush, NULL);
}

static void rna_Main_particle_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->particle, NULL);
}

static void rna_Main_palettes_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->palettes, NULL);
}

static void rna_Main_gpencil_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->gpencil, NULL);
}

static void rna_Main_wm_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->wm, NULL);
}

static void rna_Main_movieclips_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->movieclip, NULL);
}

static void rna_Main_masks_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->mask, NULL);
}

static void rna_Main_linestyle_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->linestyle, NULL);
}

static void rna_Main_cachefiles_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->cachefiles, NULL);
}

static void rna_Main_paintcurves_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->paintcurves, NULL);
}

static void rna_Main_workspaces_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->workspaces, NULL);
}

static void rna_Main_lightprobes_begin(CollectionPropertyIterator *iter, PointerRNA *ptr)
{
	Main *bmain = (Main *)ptr->data;
	rna_iterator_listbase_begin(iter, &bmain->lightprobe, NULL);
}

static int rna_ID_lookup_string(ListBase *lb, const char *key, PointerRNA *r_ptr)
{
	ID *id;
	for (id = lb->first; id != NULL; id = id->next) {
		if (STREQ(id->name + 2, key)) {
			break;
		}
		else if (strstr(key, id->name + 2) != NULL) {
			char full_name_ui[MAX_ID_FULL_NAME_UI];
			BKE_id_full_name_ui_prefix_get(full_name_ui, id);
			/* Second check skips the three 'UI keycode letters' prefix. */
			if (STREQ(full_name_ui, key) || STREQ(full_name_ui + 3, key)) {
				break;
			}
		}
	}
	if (id != NULL) {
		RNA_id_pointer_create(id, r_ptr);
		return true;
	}
	return false;
}

int rna_Main_camera_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->camera, key, r_ptr);
}

int rna_Main_scene_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->scene, key, r_ptr);
}

int rna_Main_object_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->object, key, r_ptr);
}

int rna_Main_mat_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->mat, key, r_ptr);
}

int rna_Main_nodetree_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->nodetree, key, r_ptr);
}

int rna_Main_mesh_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->mesh, key, r_ptr);
}

int rna_Main_light_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->lamp, key, r_ptr);
}

int rna_Main_library_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->library, key, r_ptr);
}

int rna_Main_screen_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->screen, key, r_ptr);
}

int rna_Main_wm_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->wm, key, r_ptr);
}

int rna_Main_image_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->image, key, r_ptr);
}

int rna_Main_latt_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->latt, key, r_ptr);
}

int rna_Main_curve_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->curve, key, r_ptr);
}

int rna_Main_mball_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->mball, key, r_ptr);
}

int rna_Main_font_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->vfont, key, r_ptr);
}

int rna_Main_tex_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->tex, key, r_ptr);
}

int rna_Main_brush_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->brush, key, r_ptr);
}

int rna_Main_world_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->world, key, r_ptr);
}

int rna_Main_collection_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->collection, key, r_ptr);
}

int rna_Main_key_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->key, key, r_ptr);
}

int rna_Main_text_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->text, key, r_ptr);
}

int rna_Main_speaker_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->speaker, key, r_ptr);
}

int rna_Main_sound_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->sound, key, r_ptr);
}

int rna_Main_armature_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->armature, key, r_ptr);
}

int rna_Main_action_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->action, key, r_ptr);
}

int rna_Main_particle_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->particle, key, r_ptr);
}

int rna_Main_palette_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->palettes, key, r_ptr);
}

int rna_Main_gpencil_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->gpencil, key, r_ptr);
}

int rna_Main_movieclip_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->movieclip, key, r_ptr);
}

int rna_Main_mask_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->mask, key, r_ptr);
}

int rna_Main_linestyle_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->linestyle, key, r_ptr);
}

int rna_Main_cachefile_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->cachefiles, key, r_ptr);
}

int rna_Main_paintcurve_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->paintcurves, key, r_ptr);
}

int rna_Main_workspace_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->workspaces, key, r_ptr);
}

int rna_Main_lightprobe_lookup_string(PointerRNA *ptr, const char *key, struct PointerRNA *r_ptr)
{
	Main *bmain = ptr->data;
	return rna_ID_lookup_string(&bmain->lightprobe, key, r_ptr);
}

static void rna_Main_version_get(PointerRNA *ptr, int *value)
{
	Main *bmain = (Main *)ptr->data;
	value[0] = bmain->versionfile / 100;
	value[1] = bmain->versionfile % 100;
	value[2] = bmain->subversionfile;
}

#ifdef UNIT_TEST

static PointerRNA rna_Test_test_get(PointerRNA *ptr)
{
	PointerRNA ret = *ptr;
	ret.type = &RNA_Test;

	return ret;
}

#endif

#else

/* local convenience types */
typedef void (CollectionDefFunc)(struct BlenderRNA *brna, struct PropertyRNA *cprop);

typedef struct MainCollectionDef {
	const char *identifier;
	const char *type;
	const char *iter_begin;
	const char *name;
	const char *description;
	CollectionDefFunc *func;
	const char *lookup_string;
} MainCollectionDef;

void RNA_def_main(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	CollectionDefFunc *func;

	/* plural must match idtypes in readblenentry.c */
	MainCollectionDef lists[] = {
		{"cameras", "Camera", "rna_Main_camera_begin", "Cameras", "Camera data-blocks", RNA_def_main_cameras, "rna_Main_camera_lookup_string"},
		{"scenes", "Scene", "rna_Main_scene_begin", "Scenes", "Scene data-blocks", RNA_def_main_scenes, "rna_Main_scene_lookup_string"},
		{"objects", "Object", "rna_Main_object_begin", "Objects", "Object data-blocks", RNA_def_main_objects, "rna_Main_object_lookup_string"},
		{"materials", "Material", "rna_Main_mat_begin", "Materials", "Material data-blocks", RNA_def_main_materials, "rna_Main_mat_lookup_string"},
		{"node_groups", "NodeTree", "rna_Main_nodetree_begin", "Node Groups", "Node group data-blocks", RNA_def_main_node_groups, "rna_Main_nodetree_lookup_string"},
		{"meshes", "Mesh", "rna_Main_mesh_begin", "Meshes", "Mesh data-blocks", RNA_def_main_meshes, "rna_Main_mesh_lookup_string"},
		{"lights", "Light", "rna_Main_light_begin", "Lights", "Light data-blocks", RNA_def_main_lights, "rna_Main_light_lookup_string"},
		{"libraries", "Library", "rna_Main_library_begin", "Libraries", "Library data-blocks", RNA_def_main_libraries, "rna_Main_library_lookup_string"},
		{"screens", "Screen", "rna_Main_screen_begin", "Screens", "Screen data-blocks", RNA_def_main_screens, "rna_Main_screen_lookup_string"},
		{"window_managers", "WindowManager", "rna_Main_wm_begin", "Window Managers", "Window manager data-blocks", RNA_def_main_window_managers, "rna_Main_wm_lookup_string"},
		{"images", "Image", "rna_Main_image_begin", "Images", "Image data-blocks", RNA_def_main_images, "rna_Main_image_lookup_string"},
		{"lattices", "Lattice", "rna_Main_latt_begin", "Lattices", "Lattice data-blocks", RNA_def_main_lattices, "rna_Main_latt_lookup_string"},
		{"curves", "Curve", "rna_Main_curve_begin", "Curves", "Curve data-blocks", RNA_def_main_curves, "rna_Main_curve_lookup_string"},
		{"metaballs", "MetaBall", "rna_Main_mball_begin", "Metaballs", "Metaball data-blocks", RNA_def_main_metaballs, "rna_Main_mball_lookup_string"},
		{"fonts", "VectorFont", "rna_Main_font_begin", "Vector Fonts", "Vector font data-blocks", RNA_def_main_fonts, "rna_Main_font_lookup_string"},
		{"textures", "Texture", "rna_Main_tex_begin", "Textures", "Texture data-blocks", RNA_def_main_textures, "rna_Main_tex_lookup_string"},
		{"brushes", "Brush", "rna_Main_brush_begin", "Brushes", "Brush data-blocks", RNA_def_main_brushes, "rna_Main_brush_lookup_string"},
		{"worlds", "World", "rna_Main_world_begin", "Worlds", "World data-blocks", RNA_def_main_worlds, "rna_Main_world_lookup_string"},
		{"collections", "Collection", "rna_Main_collection_begin", "Collections", "Collection data-blocks", RNA_def_main_collections, "rna_Main_collection_lookup_string"},
		{"shape_keys", "Key", "rna_Main_key_begin", "Shape Keys", "Shape Key data-blocks", NULL, "rna_Main_key_lookup_string"},
		{"texts", "Text", "rna_Main_text_begin", "Texts", "Text data-blocks", RNA_def_main_texts, "rna_Main_text_lookup_string"},
		{"speakers", "Speaker", "rna_Main_speaker_begin", "Speakers", "Speaker data-blocks", RNA_def_main_speakers, "rna_Main_speaker_lookup_string"},
		{"sounds", "Sound", "rna_Main_sound_begin", "Sounds", "Sound data-blocks", RNA_def_main_sounds, "rna_Main_sound_lookup_string"},
		{"armatures", "Armature", "rna_Main_armature_begin", "Armatures", "Armature data-blocks", RNA_def_main_armatures, "rna_Main_armature_lookup_string"},
		{"actions", "Action", "rna_Main_action_begin", "Actions", "Action data-blocks", RNA_def_main_actions, "rna_Main_action_lookup_string"},
		{"particles", "ParticleSettings", "rna_Main_particle_begin", "Particles", "Particle data-blocks", RNA_def_main_particles, "rna_Main_particle_lookup_string"},
		{"palettes", "Palette", "rna_Main_palettes_begin", "Palettes", "Palette data-blocks", RNA_def_main_palettes, "rna_Main_palette_lookup_string"},
		{"grease_pencil", "GreasePencil", "rna_Main_gpencil_begin", "Grease Pencil", "Grease Pencil data-blocks", RNA_def_main_gpencil, "rna_Main_gpencil_lookup_string"},
		{"movieclips", "MovieClip", "rna_Main_movieclips_begin", "Movie Clips", "Movie Clip data-blocks", RNA_def_main_movieclips, "rna_Main_movieclip_lookup_string"},
		{"masks", "Mask", "rna_Main_masks_begin", "Masks", "Masks data-blocks", RNA_def_main_masks, "rna_Main_mask_lookup_string"},
		{"linestyles", "FreestyleLineStyle", "rna_Main_linestyle_begin", "Line Styles", "Line Style data-blocks", RNA_def_main_linestyles, "rna_Main_linestyle_lookup_string"},
		{"cache_files", "CacheFile", "rna_Main_cachefiles_begin", "Cache Files", "Cache Files data-blocks", RNA_def_main_cachefiles, "rna_Main_cachefile_lookup_string"},
		{"paint_curves", "PaintCurve", "rna_Main_paintcurves_begin", "Paint Curves", "Paint Curves data-blocks", RNA_def_main_paintcurves, "rna_Main_paintcurve_lookup_string"},
		{"workspaces", "WorkSpace", "rna_Main_workspaces_begin", "Workspaces", "Workspace data-blocks", RNA_def_main_workspaces, "rna_Main_workspace_lookup_string"},
		{"lightprobes", "LightProbe", "rna_Main_lightprobes_begin", "LightProbes", "LightProbe data-blocks", RNA_def_main_lightprobes, "rna_Main_lightprobe_lookup_string"},
		{NULL, NULL, NULL, NULL, NULL, NULL}
	};

	int i;

	srna = RNA_def_struct(brna, "BlendData", NULL);
	RNA_def_struct_ui_text(srna, "Blendfile Data",
	                       "Main data structure representing a .blend file and all its data-blocks");
	RNA_def_struct_ui_icon(srna, ICON_BLENDER);

	prop = RNA_def_property(srna, "filepath", PROP_STRING, PROP_FILEPATH);
	RNA_def_property_string_maxlength(prop, FILE_MAX);
	RNA_def_property_string_funcs(prop, "rna_Main_filepath_get", "rna_Main_filepath_length", "rna_Main_filepath_set");
	RNA_def_property_clear_flag(prop, PROP_EDITABLE);
	RNA_def_property_ui_text(prop, "Filename", "Path to the .blend file");

	prop = RNA_def_property(srna, "is_dirty", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE);
	RNA_def_property_boolean_funcs(prop, "rna_Main_is_dirty_get", NULL);
	RNA_def_property_ui_text(prop, "File Has Unsaved Changes", "Have recent edits been saved to disk");

	prop = RNA_def_property(srna, "is_saved", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE);
	RNA_def_property_boolean_funcs(prop, "rna_Main_is_saved_get", NULL);
	RNA_def_property_ui_text(prop, "File is Saved", "Has the current session been saved to disk as a .blend file");

	prop = RNA_def_property(srna, "use_autopack", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_funcs(prop, "rna_Main_use_autopack_get", "rna_Main_use_autopack_set");
	RNA_def_property_ui_text(prop, "Use Autopack", "Automatically pack all external data into .blend file");

	prop = RNA_def_int_vector(srna, "version", 3, NULL, 0, INT_MAX,
	                   "Version", "Version of Blender the .blend was saved with", 0, INT_MAX);
	RNA_def_property_int_funcs(prop, "rna_Main_version_get", NULL, NULL);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE);
	RNA_def_property_flag(prop, PROP_THICK_WRAP);

	for (i = 0; lists[i].name; i++) {
		prop = RNA_def_property(srna, lists[i].identifier, PROP_COLLECTION, PROP_NONE);
		RNA_def_property_struct_type(prop, lists[i].type);
		RNA_def_property_collection_funcs(prop, lists[i].iter_begin, "rna_iterator_listbase_next",
		                                  "rna_iterator_listbase_end", "rna_iterator_listbase_get",
		                                  NULL, NULL, lists[i].lookup_string, NULL);
		RNA_def_property_ui_text(prop, lists[i].name, lists[i].description);

		/* collection functions */
		func = lists[i].func;
		if (func)
			func(brna, prop);
	}

	RNA_api_main(srna);

#ifdef UNIT_TEST

	RNA_define_verify_sdna(0);

	prop = RNA_def_property(srna, "test", PROP_POINTER, PROP_NONE);
	RNA_def_property_struct_type(prop, "Test");
	RNA_def_property_pointer_funcs(prop, "rna_Test_test_get", NULL, NULL, NULL);

	RNA_define_verify_sdna(1);

#endif
}

#endif
