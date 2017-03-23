// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_DWG_H
#define SPRK_DWG_H

#include "sprk.h"

typedef void DWGObjectID;
typedef void DWGDatabase;

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_DWG
#		define DWG_API __declspec (dllexport)
#	else
#		define DWG_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#	if defined(LINUX_SYSTEM) && defined(SPRK_DWG)
#		ifndef STATIC_APP
#			define DWG_API __attribute__ ((visibility ("default")))
#		endif
#	endif
#endif

#ifndef DWG_API
#	define DWG_API 
#endif

namespace HPS
{

/*! The DWG class contains objects and enumerations used for importing and exporting CAD files via RealDWG. */
class DWG_API DWG
{
public:
	class Layout;

	typedef std::vector<DWG::Layout, Allocator<DWG::Layout> >					LayoutArray;

	/*! The Component class is a smart pointer.  It represents a variety of DWG components.  
	 *  The primary purpose of this class is simply to provide access to the underlying DWG object
	 *	for use by DWG library functions. */
	class DWG_API Component : public HPS::Component
	{
	public:
		/*! The default constructor creates an uninitialized DWG::Component object.  The Type() function will return Type::None. */
		Component();

		/*! This constructor creates an DWG::Component object that shares the underlying smart-pointer of the source Component.  
		 *  The copy will only be successful if the source component is really an upcast of an DWG::Component object.  
		 *  Otherwise the copy will fail and the resulting DWG::Component will be invalid.
		 *	\param in_that The source Component to copy. */
		Component(HPS::Component const & in_that);

		/*! The copy constructor creates an DWG::Component object that shares the underlying smart-pointer of the source DWG::Component.
		 *	\param in_that The source DWG::Component to copy. */
		Component(DWG::Component const & in_that);

		/*! The move constructor creates an DWG::Component by transferring the underlying object of the rvalue reference to this DWG::Component.
		 * 	\param in_that An rvalue reference to an DWG::Component to take the underlying object from. */
		Component(DWG::Component && in_that);

		virtual ~Component();

		HPS::Type				ObjectType() const { return HPS::Type::DWGComponent; }

#ifndef _MSC_VER
		Component &				operator=(Component const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DWG::Component.
		 *	\param in_that An rvalue reference to an DWG::Component to take the underlying object from.
		 *	\return A reference to this DWG::Component. */
		Component &				operator=(DWG::Component && in_that);


		/*! Gets the AcDbObjectId corresponding to this DWG Component.
		 *	\return The AcDbObjectId corresponding to this DWG Component. */
		DWGObjectID	*			GetDWGObjectID() const;
	};

	/*! The Layer class is a user space object.  It allows users to turn layers imported through DWG on or off. */
	class DWG_API Layer : public DWG::Component
	{
	public:
		/*! The default constructor creates an empty Layer object. */
		Layer();

		/*! This constructor creates a Layer object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an DWG::Layer object.  Otherwise the copy will fail and the resulting DWG::Layer will be invalid.
		 *	\param in_that The source Component to copy. */
		Layer(HPS::Component const & in_that);

		/*! This constructor creates a Layer object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an DWG::Layer object.  Otherwise the copy will fail and the resulting DWG::Layer will be invalid.
		 *	\param in_that The source Component to copy. */
		Layer(Component const & in_that);

		/*! The copy constructor creates a new Layer object that contains the same settings as the source Layer.
		 * 	\param in_sheet The source Layer to copy. */
		Layer(Layer const & in_layer);

		/*! The move constructor creates an Layer by transferring the underlying impl of the rvalue reference to this Layer
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to a Layer to take the impl from. */
		Layer(Layer && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this Layer thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to a Layer to take the impl from.
		 * 	\return A reference to this Layer. */
		Layer & operator=(Layer && in_that);

		virtual ~Layer();

		HPS::Type				ObjectType() const { return HPS::Type::DWGLayer; }

		/*!	Check if the source Layer is equivalent to this Layer.
		 *	\param in_kit The source Layer to compare to this Layer.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(Layer const & in_layer) const;

		/*!	Check if the source Layer is equivalent to this Layer.
		 *	\param in_kit The source Layer to compare to this Layer.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(Layer const & in_layer) const;

		/*!	Check if the source Layer is not equivalent to this Layer.
		 *	\param in_kit The source Layer to compare to this Layer.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(Layer const & in_layer) const;

		/* Turns this Layer ON.*/
		void					TurnOn() const;

		/* Turns this Layer OFF.*/
		void					TurnOff() const;

		/* Checks whether this Layer is considered as being turned on in the Visualize scene graph
		 * \return <span class='code'>true</span> if the Layer is turned on, <span class='code'>false</span> otherwise. */
		bool					IsOn() const;
	};

	/*! The CADModel class is a smart pointer.  
	 *  It represents the DWG drawing file. */
	class DWG_API CADModel : public HPS::CADModel
	{
	public:
		/*! The default constructor creates an uninitialized CADModel object.  The Type() function will return Type::None. */
		CADModel();

		/*! This constructor creates a CADModel object that shares the underlying smart-pointer of the source Component.  
		 *  The copy will only be successful if the source component is really an upcast of an DWG::CADModel object.  
		 *  Otherwise the copy will fail and the resulting DWG::CADModel will be invalid.
		 *	\param in_that The source Component to copy. */
		CADModel(Component const & in_that);

		/*! This constructor creates an DWG::CADModel object that shares the underlying smart-pointer of the source HPS::CADModel.  
		 *  The copy will only be successful if the source component is really an upcast of an DWG::CADModel object.  
		 *  Otherwise the copy will fail and the resulting DWG::CADModel will be invalid.
		 *	\param in_that The source HPS::CADModel to copy. */
		CADModel(HPS::CADModel const & in_that);

		/*! The copy constructor creates an DWG::CADModel object that shares the underlying smart-pointer of the source DWG::CADModel.
		 *	\param in_that The source DWG::CADModel to copy. */
		CADModel(DWG::CADModel const & in_that);

		/*! The move constructor creates an DWG::CADModel by transferring the underlying object of the rvalue reference to this DWG::CADModel.
		 * 	\param in_that An rvalue reference to an DWG::CADModel to take the underlying object from. */
		CADModel(DWG::CADModel && in_that);

		virtual ~CADModel();

		HPS::Type				ObjectType() const { return HPS::Type::DWGCADModel; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		CADModel &				operator=(DWG::CADModel const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DWG::CADModel.
		 *	\param in_that An rvalue reference to an DWG::CADModel to take the underlying object from.
		 *	\return A reference to this DWG::CADModel. */
		CADModel &				operator=(DWG::CADModel && in_that);

		/*! Creates a new View object corresponding to the default layout for this CADModel.  
		 *	The View object that is returned will have the corresponding visibility settings, material settings,
		 *	transformation settings, and camera for the default capture set on it.
		 *  This call is equivalent to calling ActivateDefaultCapture
		 *	\return A newly created View object corresponding to this Layout. */
		HPS::View				ActivateDefaultLayout();

		/*! Gets the AcDbDatabase pointer corresponding to this DWG::CADModel.
		 *	\return The AcDbDatabase pointer corresponding to this DWG::CADModel. */
		DWGDatabase *			GetDWGDatabase() const;

		
		/*! Get a list of all DWG layouts from all subcomponents for this CADModel.
		*	\return A list of all DWG layouts from all subcomponents for this CADModel. */
		LayoutArray				GetAllLayouts() const;
	};

	class DWG_API Layout : public Capture
	{
		public:
		/*! The default constructor creates an uninitialized DWG::Layout object.  The Type() function will return Type::None. */
		Layout();

		/*! This constructor creates a Layout object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of a DWG::Layout object.  Otherwise the copy will fail and the resulting DWG::Layout will be invalid.
		 *	\param in_that The source Component to copy. */
		Layout(Component const & in_that);

		/*! This constructor creates an DWG::Layout object that shares the underlying smart-pointer of the source Layout.  
		 *  The copy will only be successful if the source layout is really an upcast of an DWG::Layout object.  
		 *  Otherwise the copy will fail and the resulting DWG::Layout will be invalid.
		 *	\param in_that The source Layout to copy. */
		Layout(HPS::Capture const & in_that);

		/*! The copy constructor creates an DWG::Layout object that shares the underlying smart-pointer of the source DWG::Layout.
		 *	\param in_that The source DWG::Layout to copy. */
		Layout(DWG::Layout const & in_that);

		/*! The move constructor creates an DWG::Layout by transferring the underlying object of the rvalue reference to this DWG::Layout.
		 * 	\param in_that An rvalue reference to an DWG::Layout to take the underlying object from. */
		Layout(DWG::Layout && in_that);

		virtual ~Layout();

		HPS::Type				ObjectType() const { return HPS::Type::DWGLayout; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		Layout &				operator=(Layout const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DWG::Layout.
		 *	\param in_that An rvalue reference to an DWG::Layout to take the underlying object from.
		 *	\return A reference to this DWG::Layout. */
		Layout &				operator=(DWG::Layout && in_that);


		/*! Gets the AcDbObjectId corresponding to this DWG Layout.
		 *	\return The AcDbObjectId corresponding to this DWG Layout. */
		DWGObjectID	*			GetDWGObjectID() const;
	};

	/*! The Factory class is used to create and DWG objects which inherit from HPS::Component or one of its subclasses. */
	class DWG_API Factory : public Sprocket
	{
	public:
		/*! Creates a new DWG::Component as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created DWG::Component.
		 *	\param in_type The type of DWG::Component to create.
		 *	\param in_object The DWG object ID associated with the newly created DWG::Component. */
		static Component	CreateComponent(HPS::Component const & in_owner, HPS::Component::ComponentType in_type, DWGObjectID * in_object_id = nullptr);

		/*! Creates a new DWG::CADModel.
		 *	\param in_model The Model associated with the newly created DWG::CADModel. */
		static CADModel		CreateCADModel(Model const & in_model = HPS::Factory::CreateModel());

		/*! Creates a new DWG::Layout as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created DWG::Component.
		 *	\param in_type The type of DWG::Component to create.
		 *	\param in_object The DWG object ID associated with the newly created DWG::Component. */
		static Layout		CreateLayout(HPS::Component const & in_owner, HPS::Component::ComponentType in_type, DWGObjectID * in_object_id = nullptr);

		/*! Creates a new DWG::Layer as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created DWG::Layer.
		 *	\param in_object The DWG object ID associated with the newly created DWG::Layer. */
		static Layer		CreateLayer(HPS::Component const & in_owner, DWGObjectID * in_object_id = nullptr);

	private:
		//! Private default constructor to prevent instantiation.
		Factory();
	};

	/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  
	 *  It is used to interact with an ongoing import or get the results from a completed import. */
	class DWG_API ImportNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an ImportNotifier object which is not tied to any file import. */
		ImportNotifier();

		/*! The copy constructor creates a new ImportNotifier object that is associated with the same file import as the source ImportNotifier.
		 * 	\param in_that The source ImportNotifier to copy. */
		ImportNotifier(ImportNotifier const & in_that);

		/*! The conversion constructor creates a new derived ImportNotifier object from a base IONotifier object.
		 * 	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting ImportNotifier will be invalid.
		 * 	\param in_that The source IONotifier to copy. */
		ImportNotifier(IONotifier const & in_that);

		/*! The move constructor creates an ImportNotifier by transferring the underlying impl of the rvalue reference to this ImportNotifier
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ImportNotifier to take the impl from. */
		ImportNotifier(ImportNotifier && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportNotifier thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ImportNotifier to take the impl from.
		 * 	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier && in_that);

		virtual ~ImportNotifier();

		HPS::Type				ObjectType() const { return HPS::Type::DWGImportNotifier; };

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment.
		 *	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier const & in_that);

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment. */
		void					Assign(ImportNotifier const & in_that);

		/*!	Get the CADModel for the file import.  Throws an IOException if the import is not complete, was not successful or was canceled.
		 *  \return The CADModel for a successful file import. */
		DWG::CADModel			GetCADModel() const;

		/*!	Get the number of milliseconds it took DWG to read the DWG file.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetImportTime() const;

		/*!	Get the number of milliseconds it took Visualize to parse the DWG data and create the corresponding scene graph.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetParseTime() const;
	};

	class ImportOptionsKit;

	/*!	The File class provides functions to import CAD files via DWG. */
	class DWG_API File
	{
	public:
		/*!	Performs an asynchronous import of the specified CAD file with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	\param in_file_name The name of the CAD file to import.
		 *	\param in_options The options controlling the import of the CAD file.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(char const * in_file_name, ImportOptionsKit const & in_options);

	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

	/*! The HPS::DWG::ImportOptionsKit class is a user space object.  It contains settings controlling what and how data is imported via DWG. Calling HPS::DWG::ImportOptionsKit::GetDefault() will return an options kit with values found in <a href="../../prog_guide/app/default_attribute_values.html#DWG-import-options">here.</a> */
	class DWG_API ImportOptionsKit : public HPS::SprocketKit
	{
	public:
		/*! The default constructor creates an empty ImportOptionsKit object. */
		ImportOptionsKit();

		/*! The copy constructor creates a new ImportOptionsKit object that contains the same settings as the source ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy. */
		ImportOptionsKit(ImportOptionsKit const & in_kit);

		/*! The move constructor creates an ImportOptionsKit by transferring the underlying impl of the rvalue reference to this ImportOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ImportOptionsKit to take the impl from. */
		ImportOptionsKit(ImportOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ImportOptionsKit to take the impl from.
		 * 	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit & operator=(ImportOptionsKit && in_that);

		virtual ~ImportOptionsKit();

		HPS::Type					ObjectType() const { return HPS::Type::DWGImportOptionsKit; }

		/*! Creates an ImportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import.
		 *	\return An ImportOptionsKit with the default settings. */
		static ImportOptionsKit		GetDefault();

		/*! Copies the source ImportOptionsKit into this ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy. */
		void						Set(ImportOptionsKit const & in_kit);

		/*! Copies this ImportOptionsKit into the given ImportOptionsKit.
		 * 	\param out_kit The ImportOptionsKit to populate with the contents of this ImportOptionsKit. */
		void						Show(ImportOptionsKit & out_kit) const;

		/*! Copies the source ImportOptionsKit into this ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy.
		 * 	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			operator=(ImportOptionsKit const & in_kit);

		/*! Indicates whether this ImportOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ImportOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ImportOptionsKit is equivalent to this ImportOptionsKit. 
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ImportOptionsKit const & in_kit) const;

		/*!	Check if the source ImportOptionsKit is equivalent to this ImportOptionsKit. 
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ImportOptionsKit const & in_kit) const;

		/*!	Check if the source ImportOptionsKit is not equivalent to this ImportOptionsKit. 
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ImportOptionsKit const & in_kit) const;


		/*! Sets the maximum tessellation deviation.
		*   This corresponds to the value returned by AcGiCommonDraw::deviation()
		*	\param in_deviation Maximum deviation from tessellation.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetMaxTessellationDeviation(double in_deviation);

		/*! Sets whether to import block table records which are not referenced in the drawing.  
		*	\param in_state Whether to import block table records which are not referenced in the drawing.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetUnreferencedRecords(bool in_state);

		/*! Sets whether to log internal HPS::DWG calls.  
		*	\param in_log_file File name which will contain logging information.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetLogging(const char * in_log_file);

		/*! Sets a list of directories where the importer looks for textures.
		*	\param in_texture_directories An array of directories the importer searches for textures.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTextureDirectories(HPS::UTF8Array const & in_texture_directories);

		/*! Removes the maximum tessellation deviation import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetMaxTessellationDeviation();

		/*! Removes the unreferenced records import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetUnreferencedRecords();

		/*! Removes the logging import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetLogging();

		/*! Removes the texture directory state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTextureDirectories();

		/*! Removes all settings from this ImportOptionsKit.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetEverything();

		/*! Shows the maximum tessellation deviation import setting.
		 *	\param out_deviation The maximum tessellation deviation setting for this import.
		 *	\return <span class='code'>true</span> if a maximum tessellation deviation was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMaxTessellationDeviation(double & out_deviation) const;

		/*! Shows the unreferenced records import setting.
		 *	\param out_state The unreferenced record setting for this import.
		 *	\return <span class='code'>true</span> if an unreferenced record setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowUnreferencedRecords(bool & out_state) const;

		/*! Shows the logging import setting.
		 *	\param out_log_file The file which will contain the logging information.
		 *	\return <span class='code'>true</span> if logging setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowLogging(HPS::UTF8 & out_log_file) const;

		/*! Shows the directories where the importer looks for textures.
		 *	\param out_texture_directories The directories in which the importer looks for textures.
		 *	\return <span class='code'>true</span> if texture directories were specified, <span class='code'>false</span> otherwise. */
		bool						ShowTextureDirectories(HPS::UTF8Array & out_texture_directories) const;
	};

private:
	//! Private default constructor to prevent instantiation.
	DWG();
};

}

#endif
