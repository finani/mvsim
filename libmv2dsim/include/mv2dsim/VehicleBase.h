/*+-------------------------------------------------------------------------+
  |                       MultiVehicle 2D simulator (libmv2dsim)            |
  |                                                                         |
  | Copyright (C) 2014  Jose Luis Blanco Claraco (University of Almeria)    |
  | Distributed under GNU General Public License version 3                  |
  |   See <http://www.gnu.org/licenses/>                                    |
  +-------------------------------------------------------------------------+  */

#pragma once

#include <mv2dsim/basic_types.h>
#include <mv2dsim/VisualObject.h>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Fixture.h>

namespace mv2dsim
{
	/** Virtual base class for each vehicle "actor" in the simulation. 
	  * Derived classes implements different dynamical models (Differential, Ackermann,...)
	  */
	class VehicleBase : public VisualObject
	{
	public:
		/** Class factory: Creates a vehicle from XML description of type "<vehicle>...</vehicle>".  */
		static VehicleBase* factory(World* parent, const rapidxml::xml_node<char> *xml_node);
		/// \overload
		static VehicleBase* factory(World* parent, const std::string &xml_text);

		/** Loads vehicle params from input XML node of type "<vehicle>...</vehicle>". 
		  * See derived classes & documentation for a list of accepted params.  
		  */
		void load_params_from_xml(const rapidxml::xml_node<char> *xml_node);
		/// \overload
		void load_params_from_xml(const std::string &xml_text);

		// ------- Interface with "World" ------
		/** Override to do any required process right before the integration of dynamic equations for each timestep */
		virtual void simul_pre_timestep(const TSimulContext &context) {	
			/* Default: do nothing. */
		}

		/** Override to do any required process right after the integration of dynamic equations for each timestep */
		virtual void simul_post_timestep(const TSimulContext &context) {	
			/* Default: do nothing. */
		}

		/** Gets the body dynamical state into q, dot{q} */
		void simul_post_timestep_common(const TSimulContext &context);

		/** Create bodies, fixtures, etc. for the dynamical simulation */
		virtual void create_multibody_system(b2World* world) = 0;
		
	protected:
		/** Parse node <dynamics>: The derived-class part of load_params_from_xml(), also called in factory() */
		virtual void dynamics_load_params_from_xml(const rapidxml::xml_node<char> *xml_node) = 0;

		/** Derived classes must store here the body of the vehicle main body (chassis).
		  * This is used by \a simul_post_timestep() to extract the vehicle dynamical coords (q,\dot{q}) after each simulation step.
		  */
		b2Body *m_b2d_vehicle_body;
				
		vec3 m_q;   //!< Last time-step pose (of the ref. point, in global coords)
		vec3 m_dq;  //!< Last time-step velocity (of the ref. point, in global coords)

		VehicleBase(World *parent);
	};
}