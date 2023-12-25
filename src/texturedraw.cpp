/***
 * Copyright 2004-2024, Dave Rowbotham and Toni Ylisirnio
 * All rights reserved.
 */
#include "sl.h"

/*
    software drawing primitives. draws into texture object.
    call texture_gpu_write(tex) when done to upload
    to the gpu texture object.
*/
void putpixel(texture *pixelbuffer, int32 x, int32 y, pixel32 colour)
{
    assert(pixelbuffer);
    assert(x >= 0 && x < pixelbuffer->width);
    assert(y >= 0 && y < pixelbuffer->height);
    pixelbuffer->pixels[y * pixelbuffer->width + x] = colour;
}

pixel32 getpixel(texture *pixelbuffer, int32 x, int32 y)
{
    assert(pixelbuffer);
    assert(x >= 0 && x < pixelbuffer->width);
    assert(y >= 0 && y < pixelbuffer->height);
    pixel32 colour = pixelbuffer->pixels[y * pixelbuffer->width + x];
    return colour;
}

void line(texture *pixelbuffer, int32 x1, int32 y1, int32 x2, int32 y2, pixel32 colour1, pixel32 colour2)
{
    assert(pixelbuffer);
    ivec2 p1{x1, y1};
    ivec2 p2{x2, y2};
    assert(p1.x >= 0 && p1.x < pixelbuffer->width);
    assert(p1.y >= 0 && p1.y < pixelbuffer->height);
    assert(p2.x >= 0 && p2.x < pixelbuffer->width);
    assert(p2.y >= 0 && p2.y < pixelbuffer->height);

    int32 dx, dy, sdx, sdy, px, py, dxabs, dyabs, i;
    float32 slope;

    int32 l = 0;
    int32 index = 0;
    l = std::max(std::abs(p2.x - p1.x), std::abs(p2.y - p1.y)) + 1;

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    if (dxabs >= dyabs)
    {
        pixel32 colour = colour2;
        slope = (float32)dy / (float32)dx;
        for (i = 0; i != dx; i += sdx)
        {
            uint8 r = (uint8)((float32)getr(colour1) + ((float32)index / l) * ((float32)getr(colour2) - getr(colour1)));
            uint8 g = (uint8)((float32)getg(colour1) + ((float32)index / l) * ((float32)getg(colour2) - getg(colour1)));
            uint8 b = (uint8)((float32)getb(colour1) + ((float32)index / l) * ((float32)getb(colour2) - getb(colour1)));
            colour = rgb(b, g, r);
            index++;
            px = i + p1.x;
            py = (int32)(slope * i + p1.y);
            putpixel(pixelbuffer, px, py, colour);
        }
        px = dx + p1.x;
        py = (int32)(slope * dx + p1.y);
        putpixel(pixelbuffer, px, py, colour);
    }
    else
    {
        pixel32 colour = colour2;
        slope = (float32)dx / (float32)dy;
        for (i = 0; i != dy; i += sdy)
        {
            uint8 r = (uint8)((float32)getr(colour1) + ((float32)index / l) * ((float32)getr(colour2) - getr(colour1)));
            uint8 g = (uint8)((float32)getg(colour1) + ((float32)index / l) * ((float32)getg(colour2) - getg(colour1)));
            uint8 b = (uint8)((float32)getb(colour1) + ((float32)index / l) * ((float32)getb(colour2) - getb(colour1)));
            colour = rgb(b, g, r);
            index++;
            px = (int32)(slope * i + p1.x);
            py = i + p1.y;
            putpixel(pixelbuffer, px, py, colour);
        }
        px = (int32)(slope * dy + p1.x);
        py = dy + p1.y;
        putpixel(pixelbuffer, px, py, colour);
    }
}

int32 triangle(texture *pixelbuffer, ivec2 p1, ivec2 p2, ivec2 p3,
               pixel32 colour1, pixel32 colour2, pixel32 colour3)
{
    assert(pixelbuffer);
    assert(p1.x >= 0 && p1.x < pixelbuffer->width);
    assert(p1.y >= 0 && p1.y < pixelbuffer->height);
    assert(p2.x >= 0 && p2.x < pixelbuffer->width);
    assert(p2.y >= 0 && p2.y < pixelbuffer->height);
    assert(p3.x >= 0 && p3.x < pixelbuffer->width);
    assert(p3.y >= 0 && p3.y < pixelbuffer->height);

    line(pixelbuffer, p1.x,p1.y, p2.x,p2.y, colour1, colour2);
    line(pixelbuffer, p2.x,p2.y, p3.x,p3.y, colour2, colour3);
    line(pixelbuffer, p3.x, p3.y, p1.x, p1.y, colour3, colour1);
    return 0;
}
void rectangle(texture *pixelbuffer, int32 x, int32 y, int32 width, int32 height, pixel32 colour)
{
    ivec2 p{x, y};
    ivec2 p1 = p;
    ivec2 p3{p.x + width, p.y + height};
    assert(pixelbuffer);
    assert(p.x >= 0 && p.x < pixelbuffer->width);
    assert(p.y >= 0 && p.y < pixelbuffer->height);
    assert(p3.x >= 0 && p3.x < pixelbuffer->width);
    assert(p3.y >= 0 && p3.y < pixelbuffer->height);
    ivec2 p2{p3.x, p1.y};
    ivec2 p4{p1.x, p3.y};

    line(pixelbuffer, p1.x, p1.y, p2.x, p2.y, colour, colour);
    line(pixelbuffer, p2.x, p2.y, p3.x, p3.y, colour, colour);
    line(pixelbuffer, p3.x, p3.y, p4.x, p4.y, colour, colour);
    line(pixelbuffer, p4.x, p4.y, p1.x, p1.y, colour, colour);
}
void clear(texture *pixelbuffer, pixel32 colour)
{
    assert(pixelbuffer);
    int32 count = pixelbuffer->width * pixelbuffer->height;
    pixel32 *pixels = pixelbuffer->pixels;
    uint8 r = getr(colour);
    uint8 g = getg(colour);
    uint8 b = getb(colour);
    pixel32 clearcolour = rgb(b, g, r);
    for (int32 i = 0; i < count; i++)
    {

        *pixels = clearcolour;
        pixels++;
    }
}
int32 pixelcopy(texture *destination, texture *source, ivec2 destination_origin, ivec2 source_origin, int32 width, int32 height)
{
    assert(destination);
    assert(source);
    int32 numops = width * height;
    ivec2 destination_coord = destination_origin;
    ivec2 source_coord = source_origin;
    for (int32 i = 0; i < numops; i++)
    {
        pixel32 colour;
        colour = getpixel(source, source_coord.x, source_coord.y);
        putpixel(destination, destination_coord.x, destination_coord.y, colour);
        source_coord.x++;
        destination_coord.x++;
        if (source_coord.x - source_origin.x >= width)
        {
            source_coord.x = source_origin.x;
            destination_coord.x = destination_origin.x;
            source_coord.y++;
            destination_coord.y++;
        }
        if (source_coord.y - source_origin.y >= height)
        {
            source_coord.y = source_origin.y;
            destination_coord.y = destination_origin.y;
        }
    }
    return 0;
}
int32 putpixel(textureatlas *atlas, ivec2 p, int32 index, pixel32 colour)
{
    assert(atlas);
    assert(p.x >= 0 && p.x < atlas->tile_width);
    assert(p.y >= 0 && p.y < atlas->tile_height);
    const int32 tile_x = (index % (atlas->numtiles_x) * atlas->tile_width);
    const int32 tile_y = (index / (atlas->numtiles_x) * atlas->tile_height);
    const ivec2 pos{tile_x + p.x, tile_y + p.y};
    putpixel(atlas->tex, pos.x, pos.y, colour);
    return 0;
}
int32 getpixel(textureatlas *atlas, ivec2 p, int32 index, pixel32 &colour)
{
    assert(atlas);
    assert(p.x >= 0 && p.x < atlas->tile_width);
    assert(p.y >= 0 && p.y < atlas->tile_height);
    const int32 tile_x = (index % (atlas->numtiles_x) * atlas->tile_width);
    const int32 tile_y = (index / (atlas->numtiles_x) * atlas->tile_height);
    const ivec2 pos{tile_x + p.x, tile_y + p.y};
    colour = getpixel(atlas->tex, pos.x, pos.y);
    return 0;
}
int32 line(textureatlas *atlas, ivec2 p1, ivec2 p2, int32 index, pixel32 colour1, pixel32 colour2)
{
    assert(atlas);
    assert(p1.x >= 0 && p1.x < atlas->tile_width);
    assert(p1.y >= 0 && p1.y < atlas->tile_height);
    assert(p2.x >= 0 && p2.x < atlas->tile_width);
    assert(p2.y >= 0 && p2.y < atlas->tile_height);
    const int32 tile_x = ((index % atlas->numtiles_x) * atlas->tile_width);
    const int32 tile_y = ((index / atlas->numtiles_x) * atlas->tile_height);
    const ivec2 pos1{tile_x + p1.x, tile_y + p1.y};
    const ivec2 pos2{tile_x + p2.x, tile_y + p2.y};
    line(atlas->tex, pos1.x, pos1.y, pos2.x, pos2.y, colour1, colour2);
    return 0;
}
int32 rectangle(textureatlas *atlas, ivec2 p, int32 width, int32 height, int32 index,
                pixel32 colour1, pixel32 colour2, pixel32 colour3, pixel32 colour4)
{
    ivec2 p1 = p;
    ivec2 p3{p.x + width, p.y + height};
    assert(atlas);
    assert(p1.x >= 0 && p1.x < atlas->tile_width);
    assert(p1.y >= 0 && p1.y < atlas->tile_height);
    assert(p3.x >= 0 && p3.x < atlas->tile_width);
    assert(p3.y >= 0 && p3.y < atlas->tile_height);
    ivec2 p2{p3.x, p1.y};
    ivec2 p4{p1.x, p3.y};

    line(atlas, p1, p2, index, colour1, colour2);
    line(atlas, p2, p3, index, colour2, colour3);
    line(atlas, p3, p4, index, colour3, colour4);
    line(atlas, p4, p1, index, colour4, colour1);
    return 0;
}
int32 triangle(textureatlas *atlas, ivec2 p1, ivec2 p2, ivec2 p3, int32 index,
               pixel32 colour1, pixel32 colour2, pixel32 colour3)
{
    return 0;
}
int32 clear(textureatlas *atlas, int32 index, pixel32 colour)
{
    return 0;
}