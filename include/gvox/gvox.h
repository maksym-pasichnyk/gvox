#ifndef GVOX_H
#define GVOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef enum {
    GVOX_SUCCESS = 0,
    GVOX_ERROR_FAILED_TO_LOAD_FILE = -1,
    GVOX_ERROR_FAILED_TO_LOAD_FORMAT = -2,
    GVOX_ERROR_INVALID_FORMAT = -3,
} GVoxResult;

typedef struct {
    struct {
        float x;
        float y;
        float z;
    } color;
    uint32_t id;
} GVoxVoxel;

typedef struct {
    // TODO: add a transform
    uint64_t size_x, size_y, size_z;
    GVoxVoxel *voxels;
} GVoxSceneNode;

typedef struct {
    uint64_t node_n;
    GVoxSceneNode *nodes;
} GVoxScene;

typedef struct {
    uint64_t format_name_size;
    uint64_t payload_size;
} GVoxHeader;

typedef struct {
    uint64_t size;
    uint8_t *data;
} GVoxPayload;

typedef struct {
    char const *name_str;

    void *(*create_context)(void);
    void (*destroy_context)(void *);
    GVoxPayload (*create_payload)(void *, GVoxScene const *scene);
    void (*destroy_payload)(void *, GVoxPayload const *payload);
    GVoxScene (*parse_payload)(void *, GVoxPayload const *payload);
} GVoxFormatLoaderInfo;

typedef struct _GVoxContext GVoxContext;

GVoxContext *gvox_create_context(void);
void gvox_destroy_context(GVoxContext *ctx);

#if GVOX_ENABLE_FILE_IO
size_t gvox_load_header(char const *filepath);

void gvox_push_root_path(GVoxContext *ctx, char const *path);
void gvox_pop_root_path(GVoxContext *ctx);

GVoxScene gvox_load(GVoxContext *ctx, char const *filepath);
GVoxScene gvox_load_from_raw(GVoxContext *ctx, char const *filepath, char const *src_format);
void gvox_save(GVoxContext *ctx, GVoxScene const *scene, char const *filepath, char const *dst_format);
void gvox_save_as_raw(GVoxContext *ctx, GVoxScene const *scene, char const *filepath, char const *dst_format);
#endif

void gvox_register_format(GVoxContext *ctx, GVoxFormatLoaderInfo const *format_loader_info);
void gvox_load_format(GVoxContext *ctx, char const *format_loader_name);

GVoxResult gvox_get_result(GVoxContext *ctx);
void gvox_get_result_message(GVoxContext *ctx, char *const str_buffer, size_t *str_size);
void gvox_pop_result(GVoxContext *ctx);

GVoxScene gvox_parse(GVoxContext *ctx, GVoxPayload const *payload, char const *src_format);
GVoxPayload gvox_serialize(GVoxContext *ctx, GVoxScene const *scene, char const *dst_format);
void gvox_load_raw_payload_into(GVoxContext *ctx, GVoxScene *scene, char const *dst_format, uint8_t *dst_ptr);
void gvox_serialize_into(GVoxContext *ctx, GVoxScene const *scene, char const *dst_format, uint8_t *dst_ptr);

void gvox_destroy_payload(GVoxContext *ctx, GVoxPayload const *payload, char const *format);
void gvox_destroy_scene(GVoxScene const *scene);

#ifdef __cplusplus
}
#endif

#endif
