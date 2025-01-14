/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef _ResourceGroupManager_H__
#define _ResourceGroupManager_H__

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreDataStream.h"
#include "OgreArchive.h"
#include "OgreIteratorWrappers.h"
#include "OgreCommon.h"
#include "Threading/OgreThreadHeaders.h"
#include <ctime>
#include "OgreHeaderPrefix.h"

// If X11/Xlib.h gets included before this header (for example it happens when
// including wxWidgets and FLTK), Status is defined as an int which we don't
// want as we have an enum named Status.
#ifdef Status
#undef Status
#endif

#if OGRE_RESOURCEMANAGER_STRICT == 0
#   define OGRE_RESOURCE_GROUP_INIT = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME
#elif OGRE_RESOURCEMANAGER_STRICT == 1
#   define OGRE_RESOURCE_GROUP_INIT
#else
#   define OGRE_RESOURCE_GROUP_INIT = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
#endif

namespace Ogre {

    /** \addtogroup Core
    *  @{
    */
    /** \addtogroup Resources
    *  @{
    */
    /** This class defines an interface which is called back during
        resource group loading to indicate the progress of the load. 
    @remarks
        Resource group loading is in 2 phases - creating resources from 
        declarations (which includes parsing scripts), and loading
        resources. Note that you don't necessarily have to have both; it
        is quite possible to just parse all the scripts for a group (see
        ResourceGroupManager::initialiseResourceGroup, but not to 
        load the resource group. 
        The sequence of events is (* signifies a repeating item):
        <ul>
        <li>resourceGroupScriptingStarted</li>
        <li>scriptParseStarted (*)</li>
        <li>scriptParseEnded (*)</li>
        <li>resourceGroupScriptingEnded</li>
        <li>resourceGroupLoadStarted</li>
        <li>resourceLoadStarted (*)</li>
        <li>resourceLoadEnded (*)</li>
        <li>worldGeometryStageStarted (*)</li>
        <li>worldGeometryStageEnded (*)</li>
        <li>resourceGroupLoadEnded</li>
        <li>resourceGroupPrepareStarted</li>
        <li>resourcePrepareStarted (*)</li>
        <li>resourcePrepareEnded (*)</li>
        <li>resourceGroupPrepareEnded</li>
        </ul>
    @note
        If OGRE_THREAD_SUPPORT is 1, this class is thread-safe.

    */
    class _OgreExport ResourceGroupListener
    {
    public:
        virtual ~ResourceGroupListener() {}

        /** This event is fired when a resource group begins parsing scripts.
        @note
            Remember that if you are loading resources through ResourceBackgroundQueue,
            these callbacks will occur in the background thread, so you should
            not perform any thread-unsafe actions in this callback if that's the
            case (check the group name / script name).
        @param groupName The name of the group 
        @param scriptCount The number of scripts which will be parsed
        */
        virtual void resourceGroupScriptingStarted(const String& groupName, size_t scriptCount) {}
        /** This event is fired when a script is about to be parsed.
            @param scriptName Name of the to be parsed
            @param skipThisScript A boolean passed by reference which is by default set to 
            false. If the event sets this to true, the script will be skipped and not
            parsed. Note that in this case the scriptParseEnded event will not be raised
            for this script.
        */
        virtual void scriptParseStarted(const String& scriptName, bool& skipThisScript) {}

        /** This event is fired when the script has been fully parsed.
        */
        virtual void scriptParseEnded(const String& scriptName, bool skipped) {}
        /** This event is fired when a resource group finished parsing scripts. */
        virtual void resourceGroupScriptingEnded(const String& groupName) {}

        /** This event is fired  when a resource group begins preparing.
        @param groupName The name of the group being prepared
        @param resourceCount The number of resources which will be prepared, including
            a number of stages required to prepare any linked world geometry
        */
        virtual void resourceGroupPrepareStarted(const String& groupName, size_t resourceCount)
                { (void)groupName; (void)resourceCount; }

        /** This event is fired when a declared resource is about to be prepared. 
        @param resource Weak reference to the resource prepared.
        */
        virtual void resourcePrepareStarted(const ResourcePtr& resource)
                { (void)resource; }

        /** This event is fired when the resource has been prepared. 
        */
        virtual void resourcePrepareEnded(void) {}
        /** This event is fired when a stage of preparing linked world geometry 
            is about to start. The number of stages required will have been 
            included in the resourceCount passed in resourceGroupLoadStarted.
        @param description Text description of what was just prepared
        */
        virtual void worldGeometryPrepareStageStarted(const String& description)
        { (void)description; }

        /** This event is fired when a stage of preparing linked world geometry 
            has been completed. The number of stages required will have been 
            included in the resourceCount passed in resourceGroupLoadStarted.
        */
        virtual void worldGeometryPrepareStageEnded(void) {}
        /** This event is fired when a resource group finished preparing. */
        virtual void resourceGroupPrepareEnded(const String& groupName)
        { (void)groupName; }

        /** This event is fired  when a resource group begins loading.
        @param groupName The name of the group being loaded
        @param resourceCount The number of resources which will be loaded, including
            a number of stages required to load any linked world geometry
        */
        virtual void resourceGroupLoadStarted(const String& groupName, size_t resourceCount) {}
        /** This event is fired when a declared resource is about to be loaded. 
        @param resource Weak reference to the resource loaded.
        */
        virtual void resourceLoadStarted(const ResourcePtr& resource) {}
        /** This event is fired when the resource has been loaded. 
        */
        virtual void resourceLoadEnded(void) {}
        /** This event is fired when a stage of loading linked world geometry 
            is about to start. The number of stages required will have been 
            included in the resourceCount passed in resourceGroupLoadStarted.
        @param description Text description of what was just loaded
        */
        virtual void worldGeometryStageStarted(const String& description){}
        /** This event is fired when a stage of loading linked world geometry 
            has been completed. The number of stages required will have been 
            included in the resourceCount passed in resourceGroupLoadStarted.
        */
        virtual void worldGeometryStageEnded(void) {}
        /** This event is fired when a resource group finished loading. */
        virtual void resourceGroupLoadEnded(const String& groupName) {}
        /** This event is fired when a resource was just created.
        @param resource Weak reference to the resource created.
        */
        virtual void resourceCreated(const ResourcePtr& resource)
        { (void)resource; }
        /** This event is fired when a resource is about to be removed.
        @param resource Weak reference to the resource removed.
        */
        virtual void resourceRemove(const ResourcePtr& resource)
        { (void)resource; }
    };

    /**
     @remarks   This class allows users to override resource loading behavior.
                By overriding this class' methods, you can change how resources
                are loaded and the behavior for resource name collisions.
    */
    class ResourceLoadingListener
    {
    public:
        virtual ~ResourceLoadingListener() {}

        /** This event is called when a resource beings loading. */
        virtual DataStreamPtr resourceLoading(const String &name, const String &group, Resource *resource) = 0;

        /** This event is called when a resource stream has been opened, but not processed yet. 
        @remarks
            You may alter the stream if you wish or alter the incoming pointer to point at
            another stream if you wish.
        */
        virtual void resourceStreamOpened(const String &name, const String &group, Resource *resource, DataStreamPtr& dataStream) = 0;

        /** This event is called when a resource collides with another existing one in a resource manager

            return false to skip registration of the conflicting resource and continue using the previous instance.
          */
        virtual bool resourceCollision(Resource *resource, ResourceManager *resourceManager) = 0;
    };

    /** This singleton class manages the list of resource groups, and notifying
        the various resource managers of their obligations to load / unload
        resources in a group. It also provides facilities to monitor resource
        loading per group (to do progress bars etc), provided the resources 
        that are required are pre-registered.
    @par
        Defining new resource groups,  and declaring the resources you intend to
        use in advance is optional, however it is a very useful feature. In addition, 
        if a ResourceManager supports the definition of resources through scripts, 
        then this is the class which drives the locating of the scripts and telling
        the ResourceManager to parse them. 
    @par
        There are several states that a resource can be in (the concept, not the
        object instance in this case):
        <ol>
        <li><b>Undefined</b>. Nobody knows about this resource yet. It might be
        in the filesystem, but Ogre is oblivious to it at the moment - there 
        is no Resource instance. This might be because it's never been declared
        (either in a script, or using #declareResource), or
        it may have previously been a valid Resource instance but has been 
        removed, either individually through ResourceManager::remove or as a group
        through #clearResourceGroup.</li>
        <li><b>Declared</b>. Ogre has some forewarning of this resource, either
        through calling #declareResource, or by declaring
        the resource in a script file which is on one of the resource locations
        which has been defined for a group. There is still no instance of Resource,
        but Ogre will know to create this resource when 
        #initialiseResourceGroup is called (which is automatic
        if you declare the resource group before Root::initialise).</li>
        <li><b>Unloaded</b>. There is now a Resource instance for this resource, 
        although it is not loaded. This means that code which looks for this
        named resource will find it, but the Resource is not using a lot of memory
        because it is in an unloaded state. A Resource can get into this state
        by having just been created by #initialiseResourceGroup
        (either from a script, or from a call to #declareResource), by
        being created directly from code (ResourceManager::createResource), or it may
        have previously been loaded and has been unloaded, either individually
        through Resource::unload, or as a group through #unloadResourceGroup.</li>
        <li><b>Loaded</b>The Resource instance is fully loaded. This may have
        happened implicitly because something used it, or it may have been 
        loaded as part of a group.</li>
        </ol>
    */
    class _OgreExport ResourceGroupManager : public Singleton<ResourceGroupManager>, public ResourceAlloc
    {
    public:
        OGRE_AUTO_MUTEX; // public to allow external locking
        /// Default resource group name
        static const String DEFAULT_RESOURCE_GROUP_NAME;
        /// Internal resource group name (should be used by OGRE internal only)
        static const String INTERNAL_RESOURCE_GROUP_NAME;
        /// Special resource group name which causes resource group to be automatically determined based on searching for the resource in all groups.
        static const String AUTODETECT_RESOURCE_GROUP_NAME;
        /// The number of reference counts held per resource by the resource system
        static const long RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS;
        /// Nested struct defining a resource declaration
        struct ResourceDeclaration
        {
            String resourceName;
            String resourceType;
            ManualResourceLoader* loader;
            NameValuePairList parameters;
        };
        /// List of resource declarations
        typedef list<ResourceDeclaration>::type ResourceDeclarationList;
        typedef map<String, ResourceManager*>::type ResourceManagerMap;
        typedef MapIterator<ResourceManagerMap> ResourceManagerIterator;
        /// Resource location entry
        struct ResourceLocation
        {
            /// Pointer to the archive which is the destination
            Archive* archive;
            /// Whether this location was added recursively
            bool recursive;
        };
        /// List of possible file locations
        typedef list<ResourceLocation*>::type LocationList;

    protected:
        /// Map of resource types (strings) to ResourceManagers, used to notify them to load / unload group contents
        ResourceManagerMap mResourceManagerMap;

        /// Map of loading order (Real) to ScriptLoader, used to order script parsing
        typedef multimap<Real, ScriptLoader*>::type ScriptLoaderOrderMap;
        ScriptLoaderOrderMap mScriptLoaderOrderMap;

        typedef vector<ResourceGroupListener*>::type ResourceGroupListenerList;
        ResourceGroupListenerList mResourceGroupListenerList;

        ResourceLoadingListener *mLoadingListener;

        /// Resource index entry, resourcename->location 
        typedef map<String, Archive*>::type ResourceLocationIndex;

        /// List of resources which can be loaded / unloaded
        typedef list<ResourcePtr>::type LoadUnloadResourceList;
        /// Resource group entry
        struct ResourceGroup
        {
            enum Status
            {
                UNINITIALSED = 0,
                INITIALISING = 1,
                INITIALISED = 2,
                LOADING = 3,
                LOADED = 4
            };
            /// General mutex for dealing with group content
                    OGRE_AUTO_MUTEX;
            /// Status-specific mutex, separate from content-changing mutex
                    OGRE_MUTEX(statusMutex);
            /// Group name
            String name;
            /// Group status
            Status groupStatus;
            /// List of possible locations to search
            LocationList locationList;
            /// Index of resource names to locations, built for speedy access (case sensitive archives)
            ResourceLocationIndex resourceIndexCaseSensitive;
#if !OGRE_RESOURCEMANAGER_STRICT
            /// Index of resource names to locations, built for speedy access (case insensitive archives)
            ResourceLocationIndex resourceIndexCaseInsensitive;
#endif
            /// Pre-declared resources, ready to be created
            ResourceDeclarationList resourceDeclarations;
            /// Created resources which are ready to be loaded / unloaded
            // Group by loading order of the type (defined by ResourceManager)
            // (e.g. skeletons and materials before meshes)
            typedef map<Real, LoadUnloadResourceList>::type LoadResourceOrderMap;
            LoadResourceOrderMap loadResourceOrderMap;
            /// Linked world geometry, as passed to setWorldGeometry
            String worldGeometry;
            /// Scene manager to use with linked world geometry
            SceneManager* worldGeometrySceneManager;
            // in global pool flag - if true the resource will be loaded even a different   group was requested in the load method as a parameter.
            bool inGlobalPool;

            void addToIndex(const String& filename, Archive* arch);
            void removeFromIndex(const String& filename, Archive* arch);
            void removeFromIndex(Archive* arch);

        };
        /// Map from resource group names to groups
        typedef map<String, ResourceGroup*>::type ResourceGroupMap;
        ResourceGroupMap mResourceGroupMap;

        /// Group name for world resources
        String mWorldGroupName;

        /** Parses all the available scripts found in the resource locations
        for the given group, for all ResourceManagers.
        @remarks
            Called as part of initialiseResourceGroup
        */
        void parseResourceGroupScripts(ResourceGroup* grp) const;
        /** Create all the pre-declared resources.
        @remarks
            Called as part of initialiseResourceGroup
        */
        void createDeclaredResources(ResourceGroup* grp);
        /** Adds a created resource to a group. */
        void addCreatedResource(ResourcePtr& res, ResourceGroup& group) const;
        /** Get resource group */
        ResourceGroup* getResourceGroup(const String& name) const;
        /** Drops contents of a group, leave group there, notify ResourceManagers. */
        void dropGroupContents(ResourceGroup* grp);
        /** Delete a group for shutdown - don't notify ResourceManagers. */
        void deleteGroup(ResourceGroup* grp);
        /// Internal find method for auto groups
        std::pair<Archive*, ResourceGroup*>
        resourceExistsInAnyGroupImpl(const String& filename) const;
        /// Internal event firing method
        void fireResourceGroupScriptingStarted(const String& groupName, size_t scriptCount) const;
        /// Internal event firing method
        void fireScriptStarted(const String& scriptName, bool &skipScript) const;
        /// Internal event firing method
        void fireScriptEnded(const String& scriptName, bool skipped) const;
        /// Internal event firing method
        void fireResourceGroupScriptingEnded(const String& groupName) const;
        /// Internal event firing method
        void fireResourceGroupLoadStarted(const String& groupName, size_t resourceCount) const;
        /// Internal event firing method
        void fireResourceLoadStarted(const ResourcePtr& resource) const;
        /// Internal event firing method
        void fireResourceLoadEnded(void) const;
        /// Internal event firing method
        void fireResourceGroupLoadEnded(const String& groupName) const;
        /// Internal event firing method
        void fireResourceGroupPrepareStarted(const String& groupName, size_t resourceCount) const;
        /// Internal event firing method
        void fireResourcePrepareStarted(const ResourcePtr& resource) const;
        /// Internal event firing method
        void fireResourcePrepareEnded(void) const;
        /// Internal event firing method
        void fireResourceGroupPrepareEnded(const String& groupName) const;
        /// Internal event firing method
        void fireResourceCreated(const ResourcePtr& resource) const;
        /// Internal event firing method
        void fireResourceRemove(const ResourcePtr& resource) const;
        /** Internal modification time retrieval */
        time_t resourceModifiedTime(ResourceGroup* group, const String& filename) const;

        /** Find out if the named file exists in a group. Internal use only
         @param group Pointer to the resource group
         @param filename Fully qualified name of the file to test for
         */
        Archive* resourceExists(ResourceGroup* group, const String& filename) const;

        /** Open resource with optional searching in other groups if it is not found. Internal use only */
        DataStreamPtr openResourceImpl(const String& resourceName,
            const String& groupName,
            bool searchGroupsIfNotFound,
            Resource* resourceBeingLoaded) const;

        /// Stored current group - optimisation for when bulk loading a group
        ResourceGroup* mCurrentGroup;
    public:
        ResourceGroupManager();
        virtual ~ResourceGroupManager();

        /** Create a resource group.
        @remarks
            A resource group allows you to define a set of resources that can 
            be loaded / unloaded as a unit. For example, it might be all the 
            resources used for the level of a game. There is always one predefined
            resource group called #DEFAULT_RESOURCE_GROUP_NAME,
            which is typically used to hold all resources which do not need to 
            be unloaded until shutdown. There is another predefined resource
            group called #INTERNAL_RESOURCE_GROUP_NAME too,
            which should be used by OGRE internal only, the resources created
            in this group aren't supposed to modify, unload or remove by user.
            You can create additional ones so that you can control the life of
            your resources in whichever way you wish.
            There is one other predefined value, 
            #AUTODETECT_RESOURCE_GROUP_NAME; using this
            causes the group name to be derived at load time by searching for 
            the resource in the resource locations of each group in turn.
        @par
            Once you have defined a resource group, resources which will be loaded
            as part of it are defined in one of 3 ways:
            <ol>
            <li>Manually through declareResource(); this is useful for scripted
                declarations since it is entirely generalised, and does not 
                create Resource instances right away</li>
            <li>Through the use of scripts; some ResourceManager subtypes have
                script formats (e.g. .material, .overlay) which can be used
                to declare resources</li>
            <li>By calling ResourceManager::createResource to create a resource manually.
            This resource will go on the list for it's group and will be loaded
            and unloaded with that group</li>
            </ol>
            You must remember to call #initialiseResourceGroup if you intend to use
            the first 2 types.
        @param name The name to give the resource group.
        @param inGlobalPool if true the resource will be loaded even a different
            group was requested in the load method as a parameter.
        */
        void createResourceGroup(const String& name, bool inGlobalPool = !OGRE_RESOURCEMANAGER_STRICT);


        /** Initialises a resource group.
        @remarks
            After creating a resource group, adding some resource locations, and
            perhaps pre-declaring some resources using #declareResource, but
            before you need to use the resources in the group, you 
            should call this method to initialise the group. By calling this,
            you are triggering the following processes:
            <ol>
            <li>Scripts for all resource types which support scripting are
                parsed from the resource locations, and resources within them are
                created (but not loaded yet).</li>
            <li>Creates all the resources which have just pre-declared using
            declareResource (again, these are not loaded yet)</li>
            </ol>
            So what this essentially does is create a bunch of unloaded Resource entries
            in the respective ResourceManagers based on scripts, and resources
            you've pre-declared. That means that code looking for these resources
            will find them, but they won't be taking up much memory yet, until
            they are either used, or they are loaded in bulk using loadResourceGroup.
            Loading the resource group in bulk is entirely optional, but has the 
            advantage of coming with progress reporting as resources are loaded.
        @par
            Failure to call this method means that #loadResourceGroup will do
            nothing, and any resources you define in scripts will not be found.
            Similarly, once you have called this method you won't be able to
            pick up any new scripts or pre-declared resources, unless you
            call clearResourceGroup, set up declared resources, and call this
            method again.
        @note 
            When you call Root::initialise, all resource groups that have already been
            created are automatically initialised too. Therefore you do not need to 
            call this method for groups you define and set up before you call 
            Root::initialise. However, since one of the most useful features of 
            resource groups is to set them up after the main system initialisation
            has occurred (e.g. a group per game level), you must remember to call this
            method for the groups you create after this.

        @param name The name of the resource group to initialise
        */
        void initialiseResourceGroup(const String& name);

        /** Initialise all resource groups which are yet to be initialised.
        @see #initialiseResourceGroup
        */
        void initialiseAllResourceGroups(void);

        /** Prepares a resource group.
        @remarks
            Prepares any created resources which are part of the named group.
            Note that resources must have already been created by calling
            ResourceManager::createResource, or declared using #declareResource or
            in a script (such as .material and .overlay). The latter requires
            that initialiseResourceGroup has been called. 
        
            When this method is called, this class will callback any ResourceGroupListener
            which have been registered to update them on progress. 
        @param name The name of the resource group to prepare.
        @param prepareMainResources If true, prepares normal resources associated 
            with the group (you might want to set this to false if you wanted
            to just prepare world geometry in bulk)
        @param prepareWorldGeom If true, prepares any linked world geometry
            @see #linkWorldGeometryToResourceGroup
        */
        void prepareResourceGroup(const String& name, bool prepareMainResources = true, 
            bool prepareWorldGeom = true);

        /** Loads a resource group.
        @remarks
            Loads any created resources which are part of the named group.
            Note that resources must have already been created by calling
            ResourceManager::create, or declared using declareResource() or
            in a script (such as .material and .overlay). The latter requires
            that initialiseResourceGroup has been called. 
        
            When this method is called, this class will callback any ResourceGroupListeners
            which have been registered to update them on progress. 
        @param name The name of the resource group to load.
        @param loadMainResources If true, loads normal resources associated 
            with the group (you might want to set this to false if you wanted
            to just load world geometry in bulk)
        @param loadWorldGeom If true, loads any linked world geometry
            @see #linkWorldGeometryToResourceGroup
        */
        void loadResourceGroup(const String& name, bool loadMainResources = true, 
            bool loadWorldGeom = true);

        /** Unloads a resource group.
        @remarks
            This method unloads all the resources that have been declared as
            being part of the named resource group. Note that these resources
            will still exist in their respective ResourceManager classes, but
            will be in an unloaded state. If you want to remove them entirely,
            you should use clearResourceGroup or destroyResourceGroup.
        @param name The name to of the resource group to unload.
        @param reloadableOnly If set to true, only unload the resource that is
            reloadable. Because some resources isn't reloadable, they will be
            unloaded but can't load them later. Thus, you might not want to them
            unloaded. Or, you might unload all of them, and then populate them
            manually later.
            @see Resource::isReloadable for resource is reloadable.
        */
        void unloadResourceGroup(const String& name, bool reloadableOnly = true);

        /** Unload all resources which are not referenced by any other object.
        @remarks
            This method behaves like unloadResourceGroup, except that it only
            unloads resources in the group which are not in use, ie not referenced
            by other objects. This allows you to free up some memory selectively
            whilst still keeping the group around (and the resources present,
            just not using much memory).
        @param name The name of the group to check for unreferenced resources
        @param reloadableOnly If true (the default), only unloads resources
            which can be subsequently automatically reloaded
        */
        void unloadUnreferencedResourcesInGroup(const String& name, 
            bool reloadableOnly = true);

        /** Clears a resource group. 
        @remarks
            This method unloads all resources in the group, but in addition it
            removes all those resources from their ResourceManagers, and then 
            clears all the members from the list. That means after calling this
            method, there are no resources declared as part of the named group
            any more. Resource locations still persist though.
        @param name The name to of the resource group to clear.
        */
        void clearResourceGroup(const String& name);
        
        /** Destroys a resource group, clearing it first, destroying the resources
            which are part of it, and then removing it from
            the list of resource groups. 
        @param name The name of the resource group to destroy.
        */
        void destroyResourceGroup(const String& name);

        /** Checks the status of a resource group.
        @remarks
            Looks at the state of a resource group.
            If initialiseResourceGroup has been called for the resource
            group return true, otherwise return false.
        @param name The name to of the resource group to access.
        */
        bool isResourceGroupInitialised(const String& name) const;

        /** Checks the status of a resource group.
        @remarks
            Looks at the state of a resource group.
            If loadResourceGroup has been called for the resource
            group return true, otherwise return false.
        @param name The name to of the resource group to access.
        */
        bool isResourceGroupLoaded(const String& name) const;

        /*** Verify if a resource group exists
        @param name The name of the resource group to look for
        */
        bool resourceGroupExists(const String& name) const;

        /** Adds a location to the list of searchable locations for a
            Resource type.
            @remarks
                Resource files (textures, models etc) need to be loaded from
                specific locations. By calling this method, you add another
                search location to the list. Locations added first are preferred
                over locations added later.
            @par
                Locations can be folders, compressed archives, even perhaps
                remote locations. Facilities for loading from different
                locations are provided by plugins which provide
                implementations of the Archive class.
                All the application user has to do is specify a 'loctype'
                string in order to indicate the type of location, which
                should map onto one of the provided plugins. Ogre comes
                configured with the 'FileSystem' (folders) and 'Zip' (archive
                compressed with the pkzip / WinZip etc utilities) types.
            @par
                You can also supply the name of a resource group which should
                have this location applied to it. The
                #DEFAULT_RESOURCE_GROUP_NAME group is the
                default, and one resource group which will always exist. You
                should consider defining resource groups for your more specific
                resources (e.g. per level) so that you can control loading /
                unloading better.
            @param
                name The name of the location, e.g. './data' or
                '/compressed/gamedata.zip'
            @param
                locType A string identifying the location type, e.g.
                'FileSystem' (for folders), 'Zip' etc. Must map to a
                registered plugin which deals with this type (FileSystem and
                Zip should always be available)
            @param
                groupName Type of name of the resource group which this location
                should apply to; defaults to the General group which applies to
                all non-specific resources.
            @param
                recursive If the resource location has a concept of recursive
                directory traversal, enabling this option will mean you can load
                resources in subdirectories using only their unqualified name.
                The default is to disable this so that resources in subdirectories
                with the same name are still unique.
            @param readOnly whether the Archive is read only
            @see Archive
        */
        void addResourceLocation(const String& name, const String& locType, 
            const String& resGroup = DEFAULT_RESOURCE_GROUP_NAME, bool recursive = false, bool readOnly = true);
        /** Removes a resource location from the search path. */ 
        void removeResourceLocation(const String& name, 
            const String& resGroup = DEFAULT_RESOURCE_GROUP_NAME);
        /** Verify if a resource location exists for the given group. */ 
        bool resourceLocationExists(const String& name, 
            const String& resGroup = DEFAULT_RESOURCE_GROUP_NAME) const;

        /** Declares a resource to be a part of a resource group, allowing you 
            to load and unload it as part of the group.
        @remarks
            By declaring resources before you attempt to use them, you can 
            more easily control the loading and unloading of those resources
            by their group. Declaring them also allows them to be enumerated, 
            which means events can be raised to indicate the loading progress
            (@see ResourceGroupListener). Note that another way of declaring
            resources is to use a script specific to the resource type, if
            available (e.g. .material).
        @par
            Declared resources are not created as Resource instances (and thus
            are not available through their ResourceManager) until initialiseResourceGroup
            is called, at which point all declared resources will become created 
            (but unloaded) Resource instances, along with any resources declared
            in scripts in resource locations associated with the group.
        @param name The resource name. 
        @param resourceType The type of the resource. Ogre comes preconfigured with 
            a number of resource types: 
            <ul>
            <li>Font</li>
            <li>GpuProgram</li>
            <li>HighLevelGpuProgram</li>
            <li>Material</li>
            <li>Mesh</li>
            <li>Skeleton</li>
            <li>Texture</li>
            </ul>
            .. but more can be added by plugin ResourceManager classes.
        @param groupName The name of the group to which it will belong.
        @param loadParameters A list of name / value pairs which supply custom
            parameters to the resource which will be required before it can 
            be loaded. These are specific to the resource type.
        */
        void declareResource(const String& name, const String& resourceType,
            const String& groupName = DEFAULT_RESOURCE_GROUP_NAME,
            const NameValuePairList& loadParameters = NameValuePairList());
        /** @overload
        @param loader Pointer to a ManualResourceLoader implementation which will
            be called when the Resource wishes to load. If supplied, the resource
            is manually loaded, otherwise it'll loading from file automatic.
            @note We don't support declare manually loaded resource without loader
                here, since it's meaningless.
        */
        void declareResource(const String& name, const String& resourceType,
            const String& groupName, ManualResourceLoader* loader,
            const NameValuePairList& loadParameters = NameValuePairList());
        /** Undeclare a resource.
        @remarks
            Note that this will not cause it to be unloaded
            if it is already loaded, nor will it destroy a resource which has 
            already been created if initialiseResourceGroup has been called already.
            Only unloadResourceGroup / clearResourceGroup / destroyResourceGroup 
            will do that. 
        @param name The name of the resource. 
        @param groupName The name of the group this resource was declared in. 
        */
        void undeclareResource(const String& name, const String& groupName);

        /** Open a single resource by name and return a DataStream
            pointing at the source of the data.
        @param resourceName The name of the resource to locate.
            Even if resource locations are added recursively, you
            must provide a fully qualified name to this method. You 
            can find out the matching fully qualified names by using the
            find() method if you need to.
        @param groupName The name of the resource group; this determines which 
            locations are searched. 
        @param searchGroupsIfNotFound If true, if the resource is not found in 
            the group specified, other groups will be searched. If you're
            loading a real Resource using this option, you <strong>must</strong>
            also provide the resourceBeingLoaded parameter to enable the 
            group membership to be changed
        @param resourceBeingLoaded Optional pointer to the resource being 
            loaded, which you should supply if you want
        @return Shared pointer to data stream containing the data, will be
            destroyed automatically when no longer referenced
        */
        DataStreamPtr openResource(const String& resourceName,
                                   const String& groupName = DEFAULT_RESOURCE_GROUP_NAME,
                                   Resource* resourceBeingLoaded = NULL) const
        {
            return openResourceImpl(resourceName, groupName, false, resourceBeingLoaded);
        }

        /// @deprecated use AUTODETECT_RESOURCE_GROUP_NAME instead of searchGroupsIfNotFound
        OGRE_DEPRECATED DataStreamPtr openResource(const String& resourceName,
                                                   const String& groupName,
                                                   bool searchGroupsIfNotFound,
                                                   Resource* resourceBeingLoaded = 0) const
        {
            return openResourceImpl(resourceName, groupName, searchGroupsIfNotFound, resourceBeingLoaded);
        }

        /** Open all resources matching a given pattern (which can contain
            the character '*' as a wildcard), and return a collection of 
            DataStream objects on them.
        @param pattern The pattern to look for. If resource locations have been
            added recursively, subdirectories will be searched too so this
            does not need to be fully qualified.
        @param groupName The resource group; this determines which locations
            are searched.
        @return Shared pointer to a data stream list , will be
            destroyed automatically when no longer referenced
        */
        DataStreamListPtr openResources(const String& pattern, 
            const String& groupName = DEFAULT_RESOURCE_GROUP_NAME) const;
        
        /** List all file or directory names in a resource group.
        @note
        This method only returns filenames, you can also retrieve other
        information using listFileInfo.
        @param groupName The name of the group
        @param dirs If true, directory names will be returned instead of file names
        @return A list of filenames matching the criteria, all are fully qualified
        */
        StringVectorPtr listResourceNames(const String& groupName, bool dirs = false) const;

        /** List all files in a resource group with accompanying information.
        @param groupName The name of the group
        @param dirs If true, directory names will be returned instead of file names
        @return A list of structures detailing quite a lot of information about
        all the files in the archive.
        */
        FileInfoListPtr listResourceFileInfo(const String& groupName, bool dirs = false) const;

        /** Find all file or directory names matching a given pattern in a
            resource group.
        @note
        This method only returns filenames, you can also retrieve other
        information using findFileInfo.
        @param groupName The name of the group
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of filenames matching the criteria, all are fully qualified
        */
        StringVectorPtr findResourceNames(const String& groupName, const String& pattern,
            bool dirs = false) const;

        /** Find out if the named file exists in a group. 
        @param group The name of the resource group
        @param filename Fully qualified name of the file to test for
        */
        bool resourceExists(const String& group, const String& filename) const;
        
        /** Find out if the named file exists in any group. 
        @param filename Fully qualified name of the file to test for
        */
        bool resourceExistsInAnyGroup(const String& filename) const;

        /** Find the group in which a resource exists.
        @param filename Fully qualified name of the file the resource should be
            found as
        @return Name of the resource group the resource was found in. An
            exception is thrown if the group could not be determined.
        */
        const String& findGroupContainingResource(const String& filename) const;

        /** Find all files or directories matching a given pattern in a group
            and get some detailed information about them.
        @param group The name of the resource group
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of file information structures for all files matching 
        the criteria.
        */
        FileInfoListPtr findResourceFileInfo(const String& group, const String& pattern,
            bool dirs = false) const;

        /** Retrieve the modification time of a given file */
        time_t resourceModifiedTime(const String& group, const String& filename) const;
        /** List all resource locations in a resource group.
        @param groupName The name of the group
        @return A list of resource locations matching the criteria
        */
        StringVectorPtr listResourceLocations(const String& groupName) const;

        /** Find all resource location names matching a given pattern in a
            resource group.
        @param groupName The name of the group
        @param pattern The pattern to search for; wildcards (*) are allowed
        @return A list of resource locations matching the criteria
        */
        StringVectorPtr findResourceLocation(const String& groupName, const String& pattern) const;

        /** Create a new resource file in a given group.
        @remarks
            This method creates a new file in a resource group and passes you back a 
            writeable stream. 
        @param filename The name of the file to create
        @param groupName The name of the group in which to create the file
        @param overwrite If true, an existing file will be overwritten, if false
            an error will occur if the file already exists
        @param locationPattern If the resource group contains multiple locations, 
            then usually the file will be created in the first writable location. If you 
            want to be more specific, you can include a location pattern here and 
            only locations which match that pattern (as determined by StringUtil::match)
            will be considered candidates for creation.
        */
        DataStreamPtr createResource(const String& filename, const String& groupName = DEFAULT_RESOURCE_GROUP_NAME, 
            bool overwrite = false, const String& locationPattern = BLANKSTRING);

        /** Delete a single resource file.
        @param filename The name of the file to delete. 
        @param groupName The name of the group in which to search
        @param locationPattern If the resource group contains multiple locations, 
            then usually first matching file found in any location will be deleted. If you 
            want to be more specific, you can include a location pattern here and 
            only locations which match that pattern (as determined by StringUtil::match)
            will be considered candidates for deletion.
        */
        void deleteResource(const String& filename, const String& groupName = DEFAULT_RESOURCE_GROUP_NAME, 
            const String& locationPattern = BLANKSTRING);

        /** Delete all matching resource files.
        @param filePattern The pattern (see StringUtil::match) of the files to delete. 
        @param groupName The name of the group in which to search
        @param locationPattern If the resource group contains multiple locations, 
            then usually all matching files in any location will be deleted. If you 
            want to be more specific, you can include a location pattern here and 
            only locations which match that pattern (as determined by StringUtil::match)
            will be considered candidates for deletion.
        */
        void deleteMatchingResources(const String& filePattern, const String& groupName = DEFAULT_RESOURCE_GROUP_NAME, 
            const String& locationPattern = BLANKSTRING);

        /** Adds a ResourceGroupListener which will be called back during 
            resource loading events. 
        */
        void addResourceGroupListener(ResourceGroupListener* l);
        /** Removes a ResourceGroupListener */
        void removeResourceGroupListener(ResourceGroupListener* l);

        /** Sets the resource group that 'world' resources will use.
        @remarks
            This is the group which should be used by SceneManagers implementing
            world geometry when looking for their resources. Defaults to the 
            DEFAULT_RESOURCE_GROUP_NAME but this can be altered.
        */
        void setWorldResourceGroupName(const String& groupName) {mWorldGroupName = groupName;}

        /// Gets the resource group that 'world' resources will use.
        const String& getWorldResourceGroupName(void) const { return mWorldGroupName; }

        /** Associates some world geometry with a resource group, causing it to 
            be loaded / unloaded with the resource group.
        @remarks
            You would use this method to essentially defer a call to 
            SceneManager::setWorldGeometry to the time when the resource group
            is loaded. The advantage of this is that compatible scene managers 
            will include the estimate of the number of loading stages for that
            world geometry when the resource group begins loading, allowing you
            to include that in a loading progress report. 
        @param group The name of the resource group
        @param worldGeometry The parameter which should be passed to setWorldGeometry
        @param sceneManager The SceneManager which should be called
        */
        void linkWorldGeometryToResourceGroup(const String& group, 
            const String& worldGeometry, SceneManager* sceneManager);

        /** Clear any link to world geometry from a resource group.
        @remarks
            Basically undoes a previous call to #linkWorldGeometryToResourceGroup.
        */
        void unlinkWorldGeometryFromResourceGroup(const String& group);

            /** Checks the status of a resource group.
        @remarks
            Looks at the state of a resource group.
            If loadResourceGroup has been called for the resource
            group return true, otherwise return false.
        @param name The name to of the resource group to access.
        */
        bool isResourceGroupInGlobalPool(const String& name) const;

        /** Shutdown all ResourceManagers, performed as part of clean-up. */
        void shutdownAll(void);


        /** Internal method for registering a ResourceManager (which should be
            a singleton). Creators of plugins can register new ResourceManagers
            this way if they wish.
        @remarks
            ResourceManagers that wish to parse scripts must also call 
            _registerScriptLoader.
        @param resourceType String identifying the resource type, must be unique.
        @param rm Pointer to the ResourceManager instance.
        */
        void _registerResourceManager(const String& resourceType, ResourceManager* rm);

        /** Internal method for unregistering a ResourceManager.
        @remarks
            ResourceManagers that wish to parse scripts must also call 
            _unregisterScriptLoader.
        @param resourceType String identifying the resource type.
        */
        void _unregisterResourceManager(const String& resourceType);

        /** Get an iterator over the registered resource managers.
        */
        ResourceManagerIterator getResourceManagerIterator()
        { return ResourceManagerIterator(
            mResourceManagerMap.begin(), mResourceManagerMap.end()); }

        /** Internal method for registering a ScriptLoader.
        @remarks ScriptLoaders parse scripts when resource groups are initialised.
        @param su Pointer to the ScriptLoader instance.
        */
        void _registerScriptLoader(ScriptLoader* su);

        /** Internal method for unregistering a ScriptLoader.
        @param su Pointer to the ScriptLoader instance.
        */
        void _unregisterScriptLoader(ScriptLoader* su);

        /** Method used to directly query for registered script loaders.
        @param pattern The specific script pattern (e.g. *.material) the script loader handles
        */
        ScriptLoader *_findScriptLoader(const String &pattern) const;

        /** Internal method for getting a registered ResourceManager.
        @param resourceType String identifying the resource type.
        */
        ResourceManager* _getResourceManager(const String& resourceType) const;

        /** Internal method called by ResourceManager when a resource is created.
        @param res Weak reference to resource
        */
        void _notifyResourceCreated(ResourcePtr& res) const;

        /** Internal method called by ResourceManager when a resource is removed.
        @param res Weak reference to resource
        */
        void _notifyResourceRemoved(const ResourcePtr& res) const;

        /** Internal method to notify the group manager that a resource has
            changed group (only applicable for autodetect group) */
        void _notifyResourceGroupChanged(const String& oldGroup, Resource* res) const;

        /** Internal method called by ResourceManager when all resources 
            for that manager are removed.
        @param manager Pointer to the manager for which all resources are being removed
        */
        void _notifyAllResourcesRemoved(ResourceManager* manager) const;

        /** Notify this manager that one stage of world geometry loading has been 
            started.
        @remarks
            Custom SceneManagers which load custom world geometry should call this 
            method the number of times equal to the value they return from 
            SceneManager::estimateWorldGeometry while loading their geometry.
        */
        void _notifyWorldGeometryStageStarted(const String& description) const;
        /** Notify this manager that one stage of world geometry loading has been 
            completed.
        @remarks
            Custom SceneManagers which load custom world geometry should call this 
            method the number of times equal to the value they return from 
            SceneManager::estimateWorldGeometry while loading their geometry.
        */
        void _notifyWorldGeometryStageEnded(void) const;

        /** Get a list of the currently defined resource groups. 
        @note This method intentionally returns a copy rather than a reference in
            order to avoid any contention issues in multithreaded applications.
        @return A copy of list of currently defined groups.
        */
        StringVector getResourceGroups(void) const;
        /** Get the list of resource declarations for the specified group name. 
        @note This method intentionally returns a copy rather than a reference in
            order to avoid any contention issues in multithreaded applications.
        @param groupName The name of the group
        @return A copy of list of currently defined resources.
        */
        ResourceDeclarationList getResourceDeclarationList(const String& groupName) const;

        /** Get the list of resource locations for the specified group name.
        @param groupName The name of the group
        @return The list of resource locations associated with the given group.
        */      
        const LocationList& getResourceLocationList(const String& groupName) const;

        /// Sets a new loading listener
        void setLoadingListener(ResourceLoadingListener *listener);
        /// Returns the current loading listener
        ResourceLoadingListener *getLoadingListener() const;

        /// @copydoc Singleton::getSingleton()
        static ResourceGroupManager& getSingleton(void);
        /// @copydoc Singleton::getSingleton()
        static ResourceGroupManager* getSingletonPtr(void);

    };
    /** @} */
    /** @} */
}

#include "OgreHeaderSuffix.h"

#endif
