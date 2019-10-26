/**
 * Copyright (c) 2040 Dark Energy Processor
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef LOVEWRAP_H
#define LOVEWRAP_H

// STL
#include <functional>
#include <string>

// LOVE
#include "common/Module.h"

/* love.audio */
#include "modules/audio/Audio.h"
#include "modules/audio/Source.h"

/* love.event */
#include "modules/event/Event.h"

/* love.filesystem */
#include "modules/filesystem/Filesystem.h"

/* love.font */
#include "modules/font/Font.h"

/* love.graphics */
#include "modules/graphics/Graphics.h"

/* love.math */
#include "modules/math/MathModule.h"

/* love.image */
#include "modules/image/Image.h"

/* love.math */
#include "modules/math/Transform.h"

/* love.timer */
#include "modules/timer/Timer.h"

/* love.window */
#include "modules/window/Window.h"

namespace lovewrap
{

void initialize(lua_State *L);

namespace audio
{
	using namespace love::audio;

	inline love::audio::Audio* getInstance()
	{
		return love::Module::getInstance<love::audio::Audio>(love::Module::M_AUDIO);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	/**
	 * Gets the distance model used for attenuation.
	 * @return Distance model.
	 */
	love::audio::Audio::DistanceModel getDistanceModel();
	/**
	 * Gets the current global scale factor for velocity-based doppler effects.
	 * @return The current doppler scale factor.
	 */
	float getDopplerScale();
	/**
	 * Gets the orientation of the listener.
	 * @param v A float array of size 6 containing [x,y,z] for the forward
	 * vector, followed by [x,y,z] for the up vector.
	 **/
	void getOrientation(float v[6]);
	/**
	 * Gets the position of the listener.
	 * @param v A float array of size 3 containing (x,y,z) in that order.
	 **/
	void getPosition(float v[3]);
	/**
	 * Gets the current number of simultaneous playing sources.
	 * @return The current number of simultaneous playing sources.
	 **/
	int getSourceCount();
	/**
	 * Gets the velocity of the listener.
	 * @param v A float array of size 3 containing [x,y,z] in that order.
	 **/
	void getVelocity(float v[3]);
	/**
	 * Gets the master volume.
	 * @return The current master volume.
	 **/
	float getVolume();

	/**
	 * Creates a new Source usable for real-time generated sound playback with love::audio::Source::queue.
	 * @param samplerate Number of samples per second when playing.
	 * @param bitdepth Bits per sample (8 or 16).
	 * @param channels 1 for mono or 2 for stereo.
	 * @param buffercount The number of buffers that can be queued up at any given time with
	 *                    love::audio::Source::queue. Cannot be greater than 64. A sensible default (~8)
	 *                    is chosen if no value is specified.
	 * @return The new Source usable with love::audio::Source::queue.
	 */
	love::audio::Source *newQueueableSource(int samplerate, int bitdepth, int channels, int buffercount);
	/**
	 * Creates a new Source from a filepath.
	 * @param filename The filepath to the audio file.
	 * @param type Streaming or static source.
	 * @return A new Source that can play the specified audio.
	 */
	love::audio::Source *newSource(const std::string& filename, love::audio::Source::Type type);
	/**
	 * Creates a new Source from love::filesystem::File.
	 * @param file A love::filesystem::File pointing to an audio file.
	 * @param type Streaming or static source.
	 * @return A new Source that can play the specified audio.
	*/
	love::audio::Source *newSource(love::filesystem::File *file, love::audio::Source::Type type);
	/**
	 * Creates a new Source from love::filesystem::FileData.
	 * @param filedata The love::filesystem::FileData to create a Source from.
	 * @param type Streaming or static source.
	 * @return A new Source that can play the specified audio.
	*/
	love::audio::Source *newSource(love::filesystem::FileData *filedata, love::audio::Source::Type type);
	/**
	 * Creates a new Source from love::sound::Decoder.
	 * @param decoder The love::sound::Decoder to create a Source from..
	 * @param type Streaming or static source.
	 * @return A new Source that can play the specified audio.
	 */
	love::audio::Source *newSource(love::sound::Decoder *decoder, love::audio::Source::Type type);
	/**
	 * Creates a new Source from love::sound::SoundData.
	 * @param sounddata The love::sound::SoundData to create a Source from.
	 * @return A new Source that can play the specified audio. The SourceType of the returned audio is "static".
	 */
	love::audio::Source *newSource(love::sound::SoundData *sounddata);

	/**
	 * Pauses playback on the specified source or all audio.
	 * @param source The source on which to pause the playback.
	 **/
	void pause(love::audio::Source *source = nullptr);
	/**
	 * Play the specified Source.
	 * @param source The Source to play.
	 **/
	bool play(love::audio::Source *source);
	/**
	 * Resumes playback on the specified source or all audio.
	 * @param source The source on which to resume the playback.
	 **/
	void resume(love::audio::Source *source = nullptr);
	/**
	 * Rewinds the specified source (or all audio). Whatever is playing on this
	 * source gets rewound to the start.
	 * @param source The source to rewind.
	 **/
	void rewind(love::audio::Source *source = nullptr);
	/**
	 * Sets the distance model used for attenuation.
	 * @param distanceModel Distance model.
	 */
	void setDistanceModel(love::audio::Audio::DistanceModel distanceModel);
	void setDopplerScale(float scale);
	/**
	 * Sets the orientation of the listener.
	 * @param v A float array of size 6 containing [x,y,z] for the forward
	 * vector, followed by [x,y,z] for the up vector.
	 **/
	void setOrientation(float v[6]);
	void setOrientation(float fx, float fy, float fz, float ux, float uy, float uz);
	/**
	 * Sets the position of the listener.
	 * @param v A float array of size 3 containing [x,y,z] in that order.
	 **/
	void setPosition(float v[3]);
	void setPosition(float x, float y, float z);
	/**
	 * Sets the velocity of the listener.
	 * @param v A float array of size 3 containing [x,y,z] in that order.
	 **/
	void setVelocity(float v[3]);
	void setVelocity(float x, float y, float z);
	/**
	 * Sets the master volume, where 0.0f is min (off) and 1.0f is max.
	 * @param volume The new master volume.
	 **/
	void setVolume(float volume);
	/**
	 * Stops playback on the specified source (or all audio).
	 * @param source The source on which to stop the playback.
	 **/
	void stop(love::audio::Source *source = nullptr);
}
// event namespace doesn't actually provide anything useful
// in LOVE++ :thinking:
namespace event
{
	using namespace love::event;

	inline Event* getInstance()
	{
		return love::Module::getInstance<Event>(love::Module::M_EVENT);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	void clear();
	bool poll(love::event::Message *&msg);
	void pump();
	void push(love::event::Message *msg);
	inline void quit(int exitstatus = 0)
	{
		using namespace love::event;

		std::vector<love::Variant> args;
		args.push_back(love::Variant((double)exitstatus));

		Message a("quit", args);
		push(&a);
	}
	love::event::Message *wait();
}

namespace filesystem
{
	using namespace love::filesystem;

	inline Filesystem *getInstance()
	{
		return love::Module::getInstance<Filesystem>(love::Module::M_FILESYSTEM);
	}
	/**
	 * Append data to a file, creating it if it doesn't exist.
	 * @param filename The name of the file to write to.
	 * @param data The data to append.
	 * @param size The size in bytes of the data to append.
	 */
	void append(const std::string &filename, const void *data, int64_t size);
	/**
	 * Gets whether symbolic link support is enabled.
	 * @return Whether love.filesystem follows symbolic links.
	 */
	bool areSymlinksEnabled();
	/**
	 * Recursively creates a directory.
	 * When called with "a/b" it creates both "a" and "a/b", if they don't exist already.
	 * @param name The directory to create.
	 * @return True if the directory was created, false if not.
	 * @note This function also return true if the directory is already exists.
	 */
	bool createDirectory(const std::string &name);
	/**
	 * Returns the application data directory (could be the same as getUserDirectory).
	 * @return The path of the application data directory.
	 */
	std::string getAppdataDirectory();
	/**
	 * Returns a vector with the names of files and subdirectories in the specified path.
	 * The table is not sorted in any way; the order is undefined.
	 *
	 * If the path passed to the function exists in the game and the save directory, it
	 * will list the files and directories from both places.
	 * @param dir The directory.
	 * @return A sequence with the names of all files and subdirectories as strings.
	 */
	std::vector<std::string> getDirectoryItems(const std::string &dir);
	/**
	 * Returns a vector with the names of files and subdirectories in the specified path.
	 * The table is not sorted in any way; the order is undefined.
	 *
	 * If the path passed to the function exists in the game and the save directory, it
	 * will list the files and directories from both places.
	 * @param dir The directory.
	 * @param callback A function which is called for each file and folder in the directory.
	 *                 The filename is passed to the function as an argument.
	 */
	void getDirectoryItems(const std::string &dir, std::function<void(const std::string&)> callback);
	/**
	 * Gets the write directory name for your game. Note that this only returns the name of
	 * the folder to store your files in, not the full path.
	 * @return The identity that is used as write directory.
	 */
	std::string getIdentity();
	/**
	 * Gets information about the specified file or directory.
	 * @param path The file or directory path to check.
	 * @param filtertype If supplied, this parameter causes getInfo to only return the info
	 *                   table if the item at the given path matches the specified file type.
	 * @param info Struct to contain the file information. Can be NULL.
	 * @return True if the file exists, with specified filter, and optionally store the information
	 *         to specified pointer, or false on failure.
	 */
	bool getInfo(const std::string &path, love::filesystem::Filesystem::FileType filtertype, love::filesystem::Filesystem::Info *info);
	/**
	 * Gets information about the specified file or directory.
	 * @param path The file or directory path to check.
	 * @param info Struct to contain the file information. Can be NULL.
	 * @return True if the file exists (and optionally store the information) or false on failure.
	 */
	bool getInfo(const std::string &path, love::filesystem::Filesystem::Info *info);
	/**
	 * Gets the platform-specific absolute path of the directory containing a filepath.
	 * This can be used to determine whether a file is inside the save directory or the game's source .love.
	 * @param filepath The filepath to get the directory of.
	 * @return The platform-specific full path of the directory containing the filepath, or empty string on failure.
	 */
	std::string getRealDirectory(const std::string &filepath);
	std::string getSaveDirectory();
	/**
	 * Sets the source of the game, where the code is present. This is internal function!
	 * @param source Absolute path to the game's source folder.
	 * @return True on success, false on failure.
	 */
	bool setSource(const std::string &source);
	std::string getExecutablePath();
	void setFused(bool fused);
	bool setIdentity(const std::string &identity, bool appendToPath);
}

namespace font
{
	using namespace love::font;

	inline Font *getInstance()
	{
		return love::Module::getInstance<Font>(love::Module::M_FONT);
	}
}

namespace graphics
{
	using namespace love::graphics;

	inline Graphics *getInstance()
	{
		return love::Module::getInstance<Graphics>(love::Module::M_GRAPHICS);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	const auto STACK_ALL = Graphics::StackType::STACK_ALL;
	const auto STACK_TRANSFORM = Graphics::StackType::STACK_TRANSFORM;

	void circle(Graphics::DrawMode mode, float x, float y, float radius);
	inline void circle(Graphics::DrawMode mode, love::Vector2 pos, float radius)
	{
		return circle(mode, pos.x, pos.y, radius);
	}
	void circle(Graphics::DrawMode mode, float x, float y, float radius, int segments);
	inline void circle(Graphics::DrawMode mode, love::Vector2 pos, float radius, int segments)
	{
		return circle(mode, pos.x, pos.y, radius, segments);
	}
	void clear(love::Colorf color, bool clearstencil = true, bool cleardepth = true);
	void draw(Drawable *drawable, float x = 0.0f, float y = 0.0f, float r = 0.0f, float sx = 1.0f, float sy = 1.0f, float ox = 0.0f, float oy = 0.0f, float kx = 0.0f, float ky = 0.0f);
	void draw(Texture *texture, Quad *quad, float x = 0.0f, float y = 0.0f, float r = 0.0f, float sx = 1.0f, float sy = 1.0f, float ox = 0.0f, float oy = 0.0f, float kx = 0.0f, float ky = 0.0f);
	void draw(Drawable *drawable, love::math::Transform *transform);
	void draw(Texture *texture, Quad *quad, love::math::Transform *transform);
	void points(const love::Vector2 *pos, const love::Colorf *cols, size_t amount);
	void present();
	void print(const std::string &text, float x = 0.0f, float y = 0.0f, float r = 0.0f, float sx = 1.0f, float sy = 1.0f, float ox = 0.0f, float oy = 0.0f, float kx = 0.0f, float ky = 0.0f);
	void print(std::vector<Font::ColoredString> coloredText, float x = 0.0f, float y = 0.0f, float r = 0.0f, float sx = 1.0f, float sy = 1.0f, float ox = 0.0f, float oy = 0.0f, float kx = 0.0f, float ky = 0.0f);
	void print(const std::string &text, love::math::Transform transform);
	void print(std::vector<Font::ColoredString> coloredText, love::math::Transform transform);
	void rectangle(Graphics::DrawMode mode, float x, float y, float w, float h);
	void rectangle(Graphics::DrawMode mode, float x, float y, float w, float h, float rx, float ry);
	void rectangle(Graphics::DrawMode mode, float x, float y, float w, float h, float rx, float ry, int segments);

	void push(Graphics::StackType type = STACK_TRANSFORM);
	void rotate(float r);
	void scale(float x, float y);
	void scale(float s);
	void translate(float x, float y);
	inline void translate(love::Vector2 pos)
	{
		return translate(pos.x, pos.y);
	}
	void shear(float kx, float ky);
	void origin();
	void pop();

	/* Create new Canvas with dimensions equal to the window's size in pixels. */
	Canvas *newCanvas();
	Canvas *newCanvas(Canvas::Settings &settings);
	Font *newFont(int size = 12);
	Font *newFont(const std::string &filename, int size = 12);
	Font *newFont(love::filesystem::File *file, int size = 12);
	Font *newFont(love::font::Rasterizer *rast);
	/**
	 * Creates a new Image from a filepath.
	 * @param filename The filepath to the image file.
	 * @return An Image object which can be drawn on screen.
	 */
	Image *newImage(const std::string &filename, const Image::Settings *settings = nullptr);
	Image *newImage(love::filesystem::File *file, const Image::Settings *settings = nullptr);
	Image *newImage(love::filesystem::FileData *filedata, const Image::Settings *settings = nullptr);
	Image *newImage(love::image::ImageData *imagedata, const Image::Settings *settings = nullptr);
	Image *newImage(love::image::CompressedImageData *compressedImageData, const Image::Settings *settings = nullptr);
	Mesh *newMesh(const std::vector<Mesh::AttribFormat> &vertexformat, int vertexcount, PrimitiveType drawmode, vertex::Usage usage);
	Mesh *newMesh(const std::vector<Mesh::AttribFormat> &vertexformat, const void *data, size_t datasize, PrimitiveType drawmode, vertex::Usage usage);
	Shader *newShader(const std::string &content);
	Shader *newShader(const std::string &vertex, const std::string &pixel);
	SpriteBatch *newSpriteBatch(Texture *tex, int reservedPoints = 1000, vertex::Usage usage = vertex::USAGE_DYNAMIC);
	
	love::Colorf getBackgroundColor();
	Graphics::BlendMode getBlendMode(Graphics::BlendAlpha &alphaMode);
	double getDPIScale();
	int getHeight();
	float getPointSize();
	int getWidth();
	bool isActive();

	void setBlendMode(Graphics::BlendMode blendMode, Graphics::BlendAlpha alphaMode = Graphics::BLENDALPHA_MULTIPLY);
	void setCanvas(Canvas *canvas = nullptr);
	void setCanvas(Graphics::RenderTargetsStrongRef &rts);
	void setCanvas(Graphics::RenderTargets &rts);
	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(love::Colorf color);
	void setDepthMode(CompareMode mode, bool value);
	void setDepthMode();
	void setMeshCullMode(CullMode mode);
	void setPointSize(float pz);
	void setShader(Shader *shader = nullptr);
	void setWireframe(bool enable);

	namespace shader
	{
		void sendBools(Shader *shader, const std::string &name, std::initializer_list<bool> values);
		void sendInts(Shader *shader, const std::string &name, std::initializer_list<int> values);
		void sendUInts(Shader *shader, const std::string &name, std::initializer_list<unsigned int> values);
		void sendFloats(Shader *shader, const std::string &name, std::initializer_list<float> values);
		void sendMat3(Shader *shader, const std::string &name, const love::Matrix3 &matrix);
		void sendMat4(Shader *shader, const std::string &name, const love::math::Transform *transform);
		void sendMat4(Shader *shader, const std::string &name, const love::Matrix4 &matrix); // column-major
		void sendMatrix(Shader *shader, const std::string &name, int rows, int columns, const float *data);
		void sendTextures(Shader *shader, const std::string &name, std::initializer_list<Texture*> values);
	}
}

namespace math
{
	using namespace love::math;
	inline Math *getInstance()
	{
		return love::Module::getInstance<Math>(love::Module::M_MATH);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	RandomGenerator *newRandomGenerator();
	RandomGenerator *newRandomGenerator(uint64_t seed);
}

namespace image
{
	using namespace love::image;
	inline Image *getInstance()
	{
		return love::Module::getInstance<Image>(love::Module::M_IMAGE);
	}

	ImageData *newImageData(int width, int height, love::PixelFormat pixfmt = love::PIXELFORMAT_RGBA8);
	ImageData *newImageData(const std::string &path);
}

namespace keyboard
{
	using namespace love::keyboard;
	inline Keyboard *getInstance()
	{
		return love::Module::getInstance<Keyboard>(love::Module::M_KEYBOARD);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	bool isDown(Keyboard::Key key);
	bool isDown(std::initializer_list<Keyboard::Key> keys);
}

namespace timer
{
	using namespace love::timer;
	inline Timer *getInstance()
	{
		return love::Module::getInstance<Timer>(love::Module::M_TIMER);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	double step();
	double getDelta();
	int getFPS();
}

namespace window
{
	using namespace love::window;

	inline Window *getInstance()
	{
		return love::Module::getInstance<Window>(love::Module::M_WINDOW);
	}

	inline bool isLoaded()
	{
		return getInstance() != nullptr;
	}

	inline bool getFullscreen(Window::FullscreenType &fstype)
	{
		int w, h;
		WindowSettings settings;
		getInstance()->getWindow(w, h, settings);
		fstype = settings.fstype;
		return settings.fullscreen;
	}

	inline bool getFullscreen()
	{
		Window::FullscreenType fstype;
		return getFullscreen(fstype);
	}

	inline int getVSync()
	{
		return getInstance()->getVSync();
	}

	inline bool setFullscreen(bool f)
	{
		return getInstance()->setFullscreen(f);
	}

	inline bool setMode(int width = 800, int height = 600, WindowSettings *settings = nullptr)
	{
		return getInstance()->setWindow(width, height, settings);
	}

	inline void setTitle(const std::string &title)
	{
		getInstance()->setWindowTitle(title);
	}

	inline bool setIcon(love::image::ImageData *imgd)
	{
		return getInstance()->setIcon(imgd);
	}

	inline void setVSync(int vsync)
	{
		return getInstance()->setVSync(vsync);
	}
}
}

#endif
