// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_SKETCHUP_H
#define SPRK_SKETCHUP_H

#include "sprk.h"

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_SKETCHUP
#		define SKETCHUP_API __declspec (dllexport)
#	else
#		define SKETCHUP_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#endif

#ifndef SKETCHUP_API
#	define SKETCHUP_API
#endif

namespace HPS
{

/*! The Sketchup class contains objects used for importing SketchUp files. */
class SKETCHUP_API Sketchup
{
public:

	/*! The ImportResultsKit class contains the results of a successful SketchUp import. */
	class SKETCHUP_API ImportResultsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ImportResultsKit object. */
		ImportResultsKit();

		/*! The copy constructor creates a new ImportResultsKit object that contains the same settings as the source ImportResultsKit.
		 * 	\param in_kit The source ImportResultsKit to copy. */
		ImportResultsKit(ImportResultsKit const & in_kit);

		/*! The move constructor creates an ImportResultsKit by transferring the underlying impl of the rvalue reference to this ImportResultsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ImportResultsKit to take the impl from. */
		ImportResultsKit(ImportResultsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportResultsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ImportResultsKit to take the impl from.
		 * 	\return A reference to this ImportResultsKit. */
		ImportResultsKit & operator=(ImportResultsKit && in_that);

		virtual ~ImportResultsKit();

		HPS::Type				ObjectType() const { return HPS::Type::SketchupImportResultsKit; };

		/*! Copies the source ImportResultsKit into this ImportResultsKit.
		 * 	\param in_kit The source ImportResultsKit to copy. */
		void					Set(ImportResultsKit const & in_kit);

		/*! Copies this ImportResultsKit into the given ImportResultsKit.
		 * 	\param out_kit The ImportResultsKit to populate with the contents of this ImportResultsKit. */
		void					Show(ImportResultsKit & out_kit) const;

		/*! Copies the source ImportResultsKit into this ImportResultsKit.
		 * 	\param in_kit The source ImportResultsKit to copy.
		 * 	\return A reference to this ImportResultsKit. */
		ImportResultsKit &		operator=(ImportResultsKit const & in_kit);

		/*! Indicates whether this ImportResultsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ImportResultsKit, <span class='code'>false</span> otherwise. */
		bool					Empty() const;

		/*!	Check if the source ImportResultsKit is equivalent to this ImportResultsKit. 
		 *	\param in_kit The source ImportResultsKit to compare to this ImportResultsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(ImportResultsKit const & in_kit) const;

		/*!	Check if the source ImportResultsKit is equivalent to this ImportResultsKit. 
		 *	\param in_kit The source ImportResultsKit to compare to this ImportResultsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(ImportResultsKit const & in_kit) const;

		/*!	Check if the source ImportResultsKit is not equivalent to this ImportResultsKit. 
		 *	\param in_kit The source ImportResultsKit to compare to this ImportResultsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(ImportResultsKit const & in_kit) const;

		/*! Shows the root and library segments where the SketchUp data was imported into.
		 *	\param out_target Segment the SketchUp data was imported into.
		 *	\param out_library Segment the SketchUp includes data was imported into.
		 *	\return <span class='code'>true</span> if a valid target and library segment are set, <span class='code'>false</span> otherwise. */
		bool					ShowTarget(SegmentKey & out_target, SegmentKey & out_library) const;

		/*! Shows the Model where the SketchUp data was imported into.
		 *	\param out_model Model to use to implicitly set Segment, Library and Portfolio the SketchUp data will be imported into.
		 *	\return <span class='code'>true</span> if a Model was specified on import, <span class='code'>false</span> otherwise. */
		bool					ShowTarget(Model & out_model) const;

		/*! Shows the PortfolioKey where the SketchUp data was imported into.
		 *	\param out_portfolio Portfolio the SketchUp data was imported into.
		 *	\return <span class='code'>true</span> if a valid Portfolio is set, <span class='code'>false</span> otherwise. */
		bool					ShowPortfolio(PortfolioKey & out_portfolio) const;

		/*! Shows the view where the main SketchUp camera was imported into.
		 *	\param out_view View where main SketchUp camera was imported into.
		 *	\return <span class='code'>true</span> if a View was specified on import, <span class='code'>false</span> otherwise. */
		bool					ShowView(View & out_view) const;

		/*! Shows the camera where the main SketchUp camera was imported into.
		 *	\param out_camera CameraKit where main SketchUp camera was imported into
		 *	\return <span class='code'>true</span> if a valid camera was imported, <span class='code'>false</span> otherwise. */
		bool					ShowMainCamera(HPS::CameraKit & out_camera) const;

		/*! Shows the scenes imported from SketchUp.
		 *	\param out_names UTF8Array where SketchUp scene names were imported into.
		 *	\param out_cameras CameraKitArray where SketchUp scene cameras were imported into.
		 *	\return <span class='code'>true</span> if valid names and cameras are set, <span class='code'>false</span> otherwise. */
		bool					ShowScenes(HPS::UTF8Array & out_names, HPS::CameraKitArray & out_cameras) const;
	};


	/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  It is used to interact with an ongoing import or get the results from a completed import. */
	class SKETCHUP_API ImportNotifier : public IONotifier
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
		 * 	\return A reference to this ImportNotfier. */
		ImportNotifier &		operator=(ImportNotifier && in_that);

		virtual ~ImportNotifier();

		HPS::Type				ObjectType() const { return HPS::Type::SketchupImportNotifier; };

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment.
		 *	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier const & in_that);

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment. */
		void					Assign(ImportNotifier const & in_that);

		/*!	Get the ImportResultsKit for the file import.  Throws an IOException if the import is not complete or
		 *  was not successful.
		 *  \return The ImportResultsKit for a successful file import. */
		ImportResultsKit		GetResults() const;
	};

		/*! The ImportOptionsKit class is a user space object.  It contains settings controlling what and how data is imported via Sketchup. */
	class SKETCHUP_API ImportOptionsKit : public SprocketKit
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

		HPS::Type					ObjectType() const { return HPS::Type::SketchupImportOptionsKit; }

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


		/*! Sets the root and library segments where the SketchUp data will be imported into.
		 *  If no segments are specified, root segments will be created for this purpose during import.
		 *	\param in_target Segment the SketchUp data will be imported into.
		 *	\param in_library Segment the SketchUp includes data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTarget(SegmentKey const & in_target, SegmentKey const & in_library);

		/*! Uses the Model to set the root segment, library segment, and portfolio where the SketchUp data will be imported into.
		 *	\param in_model Model to use to implicitly set Segment, Library and Portfolio the SketchUp data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTarget(Model const & in_model);

		/*! Sets the PortfolioKey where the SketchUp data will be imported into.
		 *  If no segments are specified, root segments will be created for this purpose during import.
		 *	\param in_portfolio Portfolio the SketchUp data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetPortfolio(PortfolioKey const & in_portfolio);		

		/*! Sets the View where the default SketchUp camera will be imported into.
		 *  If no View provided, camera can be retrieved via ImportResultsKit::ShowMainCamera.
		 *	\param in_view View where SketchUp camera will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetView(View const & in_view);

		/*! Sets whether to import hidden data from the SketchUp file.  Defaults to false.
		 *	\param in_state Whether to import hidden data from the SketchUp file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetHiddenData(bool in_state);

		/*! Sets whether to import metadata from the SketchUp file.  Defaults to false.
		 *	\param in_state Whether to import metadata data from the SketchUp file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetMetaData(bool in_state);


		/*! Removes the target and library segments, and Model import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTarget();

		/*! Removes the Portfolio import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPortfolio();

		/*! Removes the View import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetView();

		/*! Removes the Hidden data import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetHiddenData();

		/*! Removes the metadata import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetMetaData();

		/*! Removes all settings from this ImportOptionsKit.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetEverything();


		/*! Shows the root and library segments where the SketchUp data will be imported into.
		 *  If no segments are specified, root segments will be created for this purpose during import.
		 *	\param out_target Segment the SketchUp data will be imported into.
		 *	\param out_library Segment the SketchUp includes data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowTarget(SegmentKey & out_target, SegmentKey & out_library) const;

		/*! Shows the root and library segments and portfolio where the SketchUp data will be imported into.
		 *  If no segments are specified, root segments will be created for this purpose during import.
		 *	\param out_model Model to use to implicitly set Segment, Library and Portfolio the SketchUp data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowTarget(Model & out_model) const;

		/*! Shows the PortfolioKey where the SketchUp data will be imported into.
		 *  If no segments are specified, root segments will be created for this purpose during import.
		 *	\param out_portfolio Portfolio the SketchUp data will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowPortfolio(PortfolioKey & out_portfolio) const;		

		/*! Shows the view where the SketchUp camera will be imported into.
		 *	\param out_view View where SketchUp camera will be imported into.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowView(View & out_view) const;

		/*! Shows whether to import hidden data from the SketchUp file.  Defaults to false.
		 *	\param out_state Whether to import hidden data from the SketchUp file.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowHiddenData(bool & out_state) const;

		/*! Shows whether to import metadata from the SketchUp file.  Defaults to false.
		 *	\param out_state Whether to import metadata data from the SketchUp file.
		 *	\return A reference to this ImportOptionsKit. */
		bool						ShowMetaData(bool & out_state) const;
	};


	/*!	The File class provides functions to import SketchUp files. */
	class SKETCHUP_API File
	{
	public:
		/*!	Performs an asynchronous import of the specified SketchUp file with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 * 	\param in_file_name The name of the SketchUp file to import.
		 *	\param in_options The options controlling the import of the SketchUp file.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(char const * in_file_name, ImportOptionsKit const & in_options);
		
	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

private:
	//! Private default constructor to prevent instantiation.
	Sketchup();
};

}

#endif
